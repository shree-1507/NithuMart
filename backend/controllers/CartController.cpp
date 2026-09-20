#include "CartController.h"

#include "../services/CartService.h"

drogon::HttpResponsePtr CartController::get(
    const drogon::HttpRequestPtr &,
    int buyerId)
{
    Json::Value result =
        CartService::getCart(buyerId);

    return drogon::HttpResponse::newHttpJsonResponse(
        result);
}

drogon::HttpResponsePtr CartController::add(
    const drogon::HttpRequestPtr &request)
{
    auto json = request->getJsonObject();

    try
    {
        CartService::addToCart(*json);

        Json::Value result;
        result["message"] =
            "Product added to cart";

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

drogon::HttpResponsePtr CartController::update(
    const drogon::HttpRequestPtr &request,
    int cartId)
{
    auto json = request->getJsonObject();

    try
    {
        CartService::updateQuantity(
            cartId,
            (*json)["quantity"].asInt());

        Json::Value result;
        result["message"] =
            "Cart updated successfully";

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

drogon::HttpResponsePtr CartController::remove(
    const drogon::HttpRequestPtr &,
    int cartId)
{
    try
    {
        CartService::removeFromCart(cartId);

        Json::Value result;
        result["message"] =
            "Cart item removed";

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