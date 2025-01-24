set(TARGET_NAME ${PROJECT_NAME}-schema-fbs)

set(TARGET_DIR ${CMAKE_INSTALL_DATADIR}/roq/schema/fbs)

set(SOURCES
    api.fbs
    buffer_capacity.fbs
    cancel_all_orders.fbs
    cancel_order.fbs
    category.fbs
    connection_status.fbs
    create_order.fbs
    custom_matrix.fbs
    custom_matrix_update.fbs
    custom_metrics.fbs
    custom_metrics_update.fbs
    download_begin.fbs
    download_end.fbs
    encoding.fbs
    error.fbs
    execution_instruction.fbs
    fill.fbs
    funds_update.fbs
    gateway_settings.fbs
    gateway_status.fbs
    layer.fbs
    market_by_order_update.fbs
    market_by_price_update.fbs
    market_status.fbs
    mbo_update.fbs
    mbp_update.fbs
    measurement.fbs
    modify_order.fbs
    option_type.fbs
    order_ack.fbs
    order_status.fbs
    order_type.fbs
    order_update.fbs
    origin.fbs
    parameter.fbs
    parameters_update.fbs
    portfolio_update.fbs
    position_effect.fbs
    position.fbs
    position_update.fbs
    priority.fbs
    protocol.fbs
    rate_limit_trigger.fbs
    rate_limit_type.fbs
    ready.fbs
    reference_data.fbs
    request_status.fbs
    request_type.fbs
    risk_limit.fbs
    risk_limits.fbs
    risk_limits_update.fbs
    security_type.fbs
    side.fbs
    statistics.fbs
    statistics_type.fbs
    statistics_update.fbs
    stream_status.fbs
    support_type.fbs
    time_in_force.fbs
    top_of_book.fbs
    trade.fbs
    trade_summary.fbs
    trade_update.fbs
    trading_status.fbs
    transport.fbs
    update_action.fbs
    update_reason.fbs
    update_type.fbs)

# install

install(FILES ${SOURCES} DESTINATION ${TARGET_DIR})
