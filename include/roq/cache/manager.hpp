/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>

#include <string>
#include <utility>

#include "roq/api.hpp"

#include "roq/cache/market.hpp"

#include "roq/cache/gateway_status.hpp"
#include "roq/cache/stream_status.hpp"

namespace roq {
namespace cache {

struct Manager final {
  using MarketId = uint32_t;
  using MarketByPriceFactory =
      std::function<std::unique_ptr<MarketByPrice>(std::string_view const &exchange, std::string_view const &symbol)>;

  explicit Manager(MarketByPriceFactory const &market_by_price_factory)
      : market_by_price_factory_{market_by_price_factory} {}
  explicit Manager(MarketByPriceFactory &&market_by_price_factory)
      : market_by_price_factory_{std::move(market_by_price_factory)} {}

  // always guaranteed to return
  std::pair<Market &, bool> get_market_or_create(std::string_view const &exchange, std::string_view const &symbol) {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      auto iter = markets_.find(market_id);
      if (iter != std::end(markets_))
        return {(*iter).second, false};
      std::abort();  // must be a bug
    } else {
      auto market_id = ++next_market_id_;
      exchange_to_symbols_[exchange].emplace(symbol, market_id);
      auto res = markets_.try_emplace(market_id, market_id, exchange, symbol, market_by_price_factory_);
      assert(res.second);
      return {(*res.first).second, true};
    }
  }

  // returns false if non-existing, calls back with market if exists
  template <typename Callback>
  bool get_market(uint32_t market_id, Callback callback) {
    auto iter = markets_.find(market_id);
    if (iter == std::end(markets_))
      return false;
    callback((*iter).second);
    return true;
  }

  // returns false if non-existing, calls back with market if exists
  template <typename Callback>
  bool get_market(std::string_view const &exchange, std::string_view const &symbol, Callback callback) const {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      auto iter = markets_.find(market_id);
      if (iter != std::end(markets_)) {
        callback((*iter).second);
        return true;
      }
    }
    return false;
  }

  // returns false if non-existing, calls back with market if exists
  template <typename Callback>
  bool get_market_with_id(std::string_view const &exchange, std::string_view const &symbol, Callback callback) {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      auto iter = markets_.find(market_id);
      if (iter != std::end(markets_)) {
        callback(market_id, (*iter).second);
        return true;
      }
    }
    return false;
  }

  // returns false if non-existing, calls back with market_id if exists
  template <typename Callback>
  bool get_market_id(std::string_view const &exchange, std::string_view const &symbol, Callback callback) {
    auto market_id = find_market_id(exchange, symbol);
    if (market_id) {
      callback(market_id);
      return true;
    }
    return false;
  }

  // calls back with all markets
  template <typename Callback>
  void get_all_markets(Callback callback) {
    for (auto &[_, market] : markets_)
      callback(market);
  }
  template <typename Callback>
  void get_all_markets(Callback callback) const {
    for (auto &[_, market] : markets_)
      callback(market);
  }

  // calls back with all exchanges
  template <typename Callback>
  void get_all_exchanges(Callback callback) const {
    for (auto &[exchange, _] : exchange_to_symbols_)
      callback(exchange);
  }

  // calls back with all symbols
  template <typename Callback>
  bool get_all_symbols(std::string_view const &exchange, Callback callback) const {
    auto iter = exchange_to_symbols_.find(exchange);
    if (iter == std::end(exchange_to_symbols_))
      return false;
    auto &tmp = (*iter).second;
    for (auto &[symbol, _] : tmp)
      callback(symbol);
    return true;
  }

  [[nodiscard]] bool operator()(Event<roq::StreamStatus> const &event) {
    auto &stream_status = event.value;
    return get_stream_status(stream_status.stream_id, stream_status.account)(event);
  }

  // calls back with all stream_status
  template <typename Callback>
  bool get_all_stream_status(Callback callback) const {
    auto result = false;
    for (auto &[_, stream_status] : stream_status_) {
      result = true;
      callback(stream_status);
    }
    return result;
  }

  // calls back with all stream_status
  template <typename Callback>
  bool get_stream_status(uint16_t stream_id, Callback callback) const {
    auto iter = stream_status_.find(stream_id);
    if (iter == std::end(stream_status_))
      return false;
    callback((*iter).second);
    return true;
  }

  [[nodiscard]] bool operator()(Event<roq::GatewayStatus> const &event) {
    return get_gateway_status_or_create(event.value.account)(event);
  }

  template <typename Callback>
  bool get_gateway_status(std::string_view const &account, Callback callback) const {
    auto iter = gateway_status_.find(account);
    if (iter == std::end(gateway_status_))
      return false;
    callback((*iter).second);
    return true;
  }

  template <typename Callback>
  bool get_all_gateway_status(Callback callback) const {
    auto result = false;
    for (auto &[account, gateway_status] : gateway_status_) {
      result = true;
      callback(account, gateway_status);
    }
    return result;
  }

 protected:
  uint32_t find_market_id(std::string_view const &exchange, std::string_view const &symbol) const {
    auto iter_1 = exchange_to_symbols_.find(exchange);
    if (iter_1 == std::end(exchange_to_symbols_))
      return 0;
    auto &symbols = (*iter_1).second;
    auto iter_2 = symbols.find(symbol);
    return iter_2 == std::end(symbols) ? 0 : (*iter_2).second;
  }

  // always guaranteed to return
  StreamStatus &get_stream_status(uint16_t stream_id, std::string_view const &account) {
    auto iter = stream_status_.find(stream_id);
    if (iter == std::end(stream_status_)) {
      StreamStatus stream_status{stream_id, account};
      iter = stream_status_.emplace(stream_id, std::move(stream_status)).first;
    }
    return (*iter).second;
  }

  GatewayStatus &get_gateway_status_or_create(std::string_view const &account) {
    auto iter = gateway_status_.find(account);
    if (iter == std::end(gateway_status_)) {
      GatewayStatus gateway_status;
      iter = gateway_status_.emplace(account, std::move(gateway_status)).first;
    }
    return (*iter).second;
  }

 private:
  MarketByPriceFactory const market_by_price_factory_;
  MarketId next_market_id_ = 0;
  absl::flat_hash_map<Exchange, absl::flat_hash_map<Symbol, MarketId>> exchange_to_symbols_;
  absl::node_hash_map<MarketId, Market> markets_;
  absl::node_hash_map<uint16_t, StreamStatus> stream_status_;
  absl::node_hash_map<Account, GatewayStatus> gateway_status_;
};

}  // namespace cache
}  // namespace roq
