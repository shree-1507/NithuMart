#pragma once

#include <drogon/drogon.h>

class OrderController
{
public:
    static drogon::HttpResponsePtr place(
        const drogon::HttpRequestPtr &request);

    static drogon::HttpResponsePtr buyerOrders(
        const drogon::HttpRequestPtr &request,
        int buyerId);

    static drogon::HttpResponsePtr sellerOrders(
        const drogon::HttpRequestPtr &request,
        int sellerId);

    static drogon::HttpResponsePtr status(
        const drogon::HttpRequestPtr &request,
        int orderId);
};