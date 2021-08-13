/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {

//! Order matching interface used to manage a simulated order book
class ROQ_PUBLIC Matcher {
 public:
  //! Dispatch interface used to handle (possibly processed) events
  class ROQ_PUBLIC Dispatcher {
   public:
    virtual void operator()(const Event<DownloadBegin> &) = 0;
    virtual void operator()(const Event<DownloadEnd> &) = 0;

    // config
    virtual void operator()(const Event<GatewaySettings> &) = 0;

    // stream
    virtual void operator()(const Event<StreamStatus> &) = 0;

    // service
    virtual void operator()(const Event<GatewayStatus> &) = 0;

    // market data
    virtual void operator()(const Event<ReferenceData> &) = 0;
    virtual void operator()(const Event<MarketStatus> &) = 0;
    virtual void operator()(const Event<TopOfBook> &) = 0;
    virtual void operator()(const Event<MarketByPriceUpdate> &) = 0;
    virtual void operator()(const Event<MarketByOrderUpdate> &) = 0;
    virtual void operator()(const Event<TradeSummary> &) = 0;
    virtual void operator()(const Event<StatisticsUpdate> &) = 0;

    // order management
    virtual void operator()(const Event<OrderAck> &) = 0;
    virtual void operator()(const Event<OrderUpdate> &) = 0;
    virtual void operator()(const Event<TradeUpdate> &) = 0;

    // account management
    virtual void operator()(const Event<PositionUpdate> &) = 0;
    virtual void operator()(const Event<FundsUpdate> &) = 0;
  };

  virtual ~Matcher() {}

  virtual std::string_view get_source_name() const = 0;

  virtual std::chrono::nanoseconds get_market_data_latency() const = 0;
  virtual std::chrono::nanoseconds get_order_management_latency() const = 0;

  //! Process inbound messages
  virtual void process(std::chrono::nanoseconds next_receive_time) = 0;

  //! Dispatch outbound messages
  virtual std::pair<bool, std::chrono::nanoseconds> peek_next_receive_time(
      std::chrono::nanoseconds next_receive_time) = 0;
  virtual void dispatch(Dispatcher &, std::chrono::nanoseconds next_receive_time) = 0;

  // config
  virtual void operator()(const Event<GatewaySettings> &) = 0;

  // stream
  virtual void operator()(const Event<StreamStatus> &) = 0;

  // service
  virtual void operator()(const Event<GatewayStatus> &) = 0;

  // market data
  virtual void operator()(const Event<ReferenceData> &) = 0;
  virtual void operator()(const Event<MarketStatus> &) = 0;
  virtual void operator()(const Event<TopOfBook> &) = 0;
  virtual void operator()(const Event<MarketByPriceUpdate> &) = 0;
  virtual void operator()(const Event<MarketByOrderUpdate> &) = 0;
  virtual void operator()(const Event<TradeSummary> &) = 0;
  virtual void operator()(const Event<StatisticsUpdate> &) = 0;

  // account management
  virtual void operator()(const Event<PositionUpdate> &) = 0;
  virtual void operator()(const Event<FundsUpdate> &) = 0;

  // client request
  virtual void operator()(const Event<CreateOrder> &) = 0;
  virtual void operator()(const Event<ModifyOrder> &) = 0;
  virtual void operator()(const Event<CancelOrder> &) = 0;
  virtual void operator()(const Event<CancelAllOrders> &) = 0;
};

}  // namespace roq
