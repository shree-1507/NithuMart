#include "ProductController.h"

#include "../services/ProductService.h"

drogon::HttpResponsePtr ProductController::getAll(
    const drogon::HttpRequestPtr &)
{
    Json::Value result =
        ProductService::getAllProducts();

    return drogon::HttpResponse::newHttpJsonResponse(
        result);
}

drogon::HttpResponsePtr ProductController::getOne(
    const drogon::HttpRequestPtr &,
    int id)
{
    try
    {
        Json::Value result =
            ProductService::getProduct(id);

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

drogon::HttpResponsePtr ProductController::add(
    const drogon::HttpRequestPtr &request)
{
    auto json = request->getJsonObject();

    try
    {
        Json::Value result =
            ProductService::addProduct(*json);

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

drogon::HttpResponsePtr ProductController::update(
    const drogon::HttpRequestPtr &request,
    int id)
{
    auto json = request->getJsonObject();

    try
    {
        Json::Value result =
            ProductService::updateProduct(id, *json);

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

drogon::HttpResponsePtr ProductController::remove(
    const drogon::HttpRequestPtr &,
    int id)
{
    try
    {
        ProductService::deleteProduct(id);

        Json::Value result;
        result["message"] =
            "Product deleted successfully";

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