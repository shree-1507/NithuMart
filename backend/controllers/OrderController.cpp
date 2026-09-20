#include "OrderController.h"

#include "../services/OrderService.h"

drogon::HttpResponsePtr OrderController::place(
    const drogon::HttpRequestPtr &request)
{
    auto json = request->getJsonObject();

    try
    {
        int buyerId =
            (*json)["buyer_id"].asInt();

        Json::Value result =
            OrderService::placeOrder(buyerId);

        return drogon::HttpResponse::newHttpJsonResponse(
            result);
    }
    catch (const std::exception &e)
    {
        Json::Value error;
        error["error"] = e.what();

        return drogon::HttpResponse::newHttpJsonResponse(
            error);
    }
}

drogon::HttpResponsePtr OrderController::buyerOrders(
    const drogon::HttpRequestPtr &,
    int buyerId)
{
    Json::Value result =
        OrderService::getBuyerOrders(buyerId);

    return drogon::HttpResponse::newHttpJsonResponse(
        result);
}

drogon::HttpResponsePtr OrderController::sellerOrders(
    const drogon::HttpRequestPtr &,
    int sellerId)
{
    Json::Value result =
        OrderService::getSellerOrders(sellerId);

    return drogon::HttpResponse::newHttpJsonResponse(
        result);
}

drogon::HttpResponsePtr OrderController::status(
    const drogon::HttpRequestPtr &request,
    int orderId)
{
    auto json = request->getJsonObject();

    try
    {
        OrderService::updateStatus(
            orderId,
            (*json)["status"].asString());

        Json::Value result;
        result["message"] =
            "Order status updated";

        return drogon::HttpResponse::newHttpJsonResponse(
            result);
    }
    catch (const std::exception &e)
    {
        Json::Value error;
        error["error"] = e.what();

        return drogon::HttpResponse::newHttpJsonResponse(
            error);
    }
}