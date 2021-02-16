/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"

namespace roq {

//! Identifies the end of a download sequence
struct ROQ_PUBLIC DownloadEnd final {
  DownloadEnd() = default;
  DownloadEnd(DownloadEnd &&) = default;
  DownloadEnd(const DownloadEnd &) = delete;

  std::string_view account;    //!< Account name (as known to the gateway)
  uint32_t max_order_id = {};  //!< Highest previous order identifier (as seen by gateway)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::DownloadEnd> : public roq::formatter {
  template <typename Context>
  auto format(const roq::DownloadEnd &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(max_order_id={})"
        R"(}})"_fmt,
        value.account,
        value.max_order_id);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::DownloadEnd> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::DownloadEnd> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(download_end={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};