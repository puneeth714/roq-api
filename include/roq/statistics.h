/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/uuid.h"

#include "roq/statistics_type.h"

namespace roq {

//! Represents a single statistic
struct ROQ_PUBLIC Statistics final {
  StatisticsType type = StatisticsType::UNDEFINED;  //!< Statistics type
  double value = NaN;                               //!< Value
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Statistics> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Statistics &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(type={}, )"
        R"(value={})"
        R"(}})"_fmt,
        value.type,
        value.value);
  }
};