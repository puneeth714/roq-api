/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct Funds final {
  Funds() = default;

  Funds(Funds const &) = delete;
  Funds(Funds &&) = default;

  void clear() {
    stream_id = {};
    balance = NaN;
    hold = NaN;
    external_account.clear();
    exchange_time_utc = {};
  }

  [[nodiscard]] bool operator()(FundsUpdate const &funds_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, funds_update.stream_id);
    dirty |= utils::update_if_not_empty(balance, funds_update.balance);
    dirty |= utils::update_if_not_empty(hold, funds_update.hold);
    dirty |= utils::update_if_not_empty(external_account, funds_update.external_account);
    dirty |= utils::update_if_not_empty(exchange_time_utc, funds_update.exchange_time_utc);
    return dirty;
  }

  template <typename Context>
  [[nodiscard]] FundsUpdate convert(Context const &context) const {
    return {
        .stream_id = stream_id,
        .account = context.account,
        .currency = context.symbol,
        .balance = balance,
        .hold = hold,
        .external_account = external_account,
        .update_type = UpdateType::SNAPSHOT,
        .exchange_time_utc = exchange_time_utc,
        .sending_time_utc = {},
    };
  }

  uint16_t stream_id = {};

  double balance = NaN;
  double hold = NaN;
  ExternalAccount external_account;
  std::chrono::nanoseconds exchange_time_utc = {};
};

}  // namespace cache
}  // namespace roq
