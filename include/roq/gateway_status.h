/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Update relating to current gateway service status
struct ROQ_PUBLIC GatewayStatus final {
  std::string_view account;   //!< Account name (as known to the gateway)
  uint64_t supported = {};    //!< Supported update types (bit mask)
  uint64_t available = {};    //!< Update types (bit mask) available from all streams
  uint64_t unavailable = {};  //!< Update types (bit mask) unavailable from one or more streams
};

}  // namespace roq

template <>
struct fmt::formatter<roq::GatewayStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::GatewayStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(supported={:#x}, )"
        R"(available={:#x}, )"
        R"(unavailable={:#x})"
        R"(}})"_fmt,
        value.account,
        value.supported,
        value.available,
        value.unavailable);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::GatewayStatus> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::GatewayStatus> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(gateway_status={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
