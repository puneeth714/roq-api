/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

//! Order (state)
struct ROQ_PUBLIC Order final {
  Order(uint32_t order_id) : order_id{order_id} {}

  Order(Order const &) = delete;
  Order(Order &&) = default;

  void clear() {
    stream_id = {};
    side = {};
    position_effect = {};
    max_show_quantity = NaN;
    order_type = {};
    time_in_force = {};
    execution_instructions = {};
    create_time_utc = {};
    update_time_utc = {};
    external_account.clear();
    external_order_id.clear();
    client_order_id.clear();
    status = {};
    quantity = NaN;
    price = NaN;
    stop_price = NaN;
    remaining_quantity = NaN;
    traded_quantity = NaN;
    average_traded_price = NaN;
    last_traded_quantity = NaN;
    last_traded_price = NaN;
    last_liquidity = {};
    routing_id.clear();
    max_request_version = {};
    max_response_version = {};
    max_accepted_version = {};
    user.clear();
  }

  [[nodiscard]] bool operator()(OrderUpdate const &order_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, order_update.stream_id);
    dirty |= utils::update_if_not_empty(side, order_update.side);
    dirty |= utils::update_if_not_empty(position_effect, order_update.position_effect);
    dirty |= utils::update_if_not_empty(max_show_quantity, order_update.max_show_quantity);
    dirty |= utils::update_if_not_empty(order_type, order_update.order_type);
    dirty |= utils::update_if_not_empty(time_in_force, order_update.time_in_force);
    dirty |= utils::update_if_not_empty(execution_instructions, order_update.execution_instructions);
    dirty |= utils::update_if_not_empty(create_time_utc, order_update.create_time_utc);
    dirty |= utils::update_if_not_empty(update_time_utc, order_update.update_time_utc);
    dirty |= utils::update_if_not_empty(external_account, order_update.external_account);
    dirty |= utils::update_if_not_empty(external_order_id, order_update.external_order_id);
    dirty |= utils::update_if_not_empty(client_order_id, order_update.client_order_id);
    dirty |= utils::update(status, order_update.status);
    dirty |= utils::update(quantity, order_update.quantity);
    dirty |= utils::update(price, order_update.price);
    dirty |= utils::update(stop_price, order_update.stop_price);
    dirty |= utils::update(remaining_quantity, order_update.remaining_quantity);
    dirty |= utils::update(traded_quantity, order_update.traded_quantity);
    dirty |= utils::update(average_traded_price, order_update.average_traded_price);
    dirty |= utils::update(last_traded_quantity, order_update.last_traded_quantity);
    dirty |= utils::update(last_traded_price, order_update.last_traded_price);
    dirty |= utils::update(last_liquidity, order_update.last_liquidity);
    dirty |= utils::update(routing_id, order_update.routing_id);
    dirty |= utils::update(max_request_version, order_update.max_request_version);
    dirty |= utils::update(max_response_version, order_update.max_response_version);
    dirty |= utils::update(max_accepted_version, order_update.max_accepted_version);
    dirty |= utils::update_if_not_empty(user, order_update.user);
    // some update types will always be published
    switch (order_update.update_type) {
      using enum UpdateType;
      case UNDEFINED:
        break;
      case SNAPSHOT:
        dirty = true;
        break;
      case INCREMENTAL:
        break;
      case STALE:
        dirty = true;
        break;
    }
    return dirty;
  }

  template <typename Context>
  [[nodiscard]] OrderUpdate convert(Context const &context) const {
    return {
        .stream_id = stream_id,
        .account = context.account,
        .order_id = order_id,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .side = side,
        .position_effect = position_effect,
        .max_show_quantity = max_show_quantity,
        .order_type = order_type,
        .time_in_force = time_in_force,
        .execution_instructions = execution_instructions,
        .create_time_utc = create_time_utc,
        .update_time_utc = update_time_utc,
        .external_account = external_account,
        .external_order_id = external_order_id,
        .client_order_id = client_order_id,
        .status = status,
        .quantity = quantity,
        .price = price,
        .stop_price = stop_price,
        .remaining_quantity = remaining_quantity,
        .traded_quantity = traded_quantity,
        .average_traded_price = average_traded_price,
        .last_traded_quantity = last_traded_quantity,
        .last_traded_price = last_traded_price,
        .last_liquidity = last_liquidity,
        .routing_id = routing_id,
        .max_request_version = max_request_version,
        .max_response_version = max_response_version,
        .max_accepted_version = max_accepted_version,
        .update_type = UpdateType::SNAPSHOT,  // note!
        .user = user,
    };
  }

  uint16_t stream_id = {};

  uint32_t const order_id = {};
  Side side = {};
  PositionEffect position_effect = {};
  double max_show_quantity = NaN;
  OrderType order_type = {};
  TimeInForce time_in_force = {};
  Mask<ExecutionInstruction> execution_instructions;
  std::chrono::nanoseconds create_time_utc = {};
  std::chrono::nanoseconds update_time_utc = {};
  ExternalAccount external_account;
  ExternalOrderId external_order_id;
  ClOrdId client_order_id;
  OrderStatus status = {};
  double quantity = NaN;
  double price = NaN;
  double stop_price = NaN;
  double remaining_quantity = NaN;
  double traded_quantity = NaN;
  double average_traded_price = NaN;
  double last_traded_quantity = NaN;
  double last_traded_price = NaN;
  Liquidity last_liquidity = {};
  RoutingId routing_id;
  uint32_t max_request_version = {};
  uint32_t max_response_version = {};
  uint32_t max_accepted_version = {};
  User user;
};

}  // namespace cache
}  // namespace roq
