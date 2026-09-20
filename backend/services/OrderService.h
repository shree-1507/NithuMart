#pragma once

#include <json/json.h>

namespace OrderService
{
    Json::Value placeOrder(int buyerId);
    Json::Value getBuyerOrders(int buyerId);
    Json::Value getSellerOrders(int sellerId);
    void updateStatus(int orderId,
                      const std::string &status);
}