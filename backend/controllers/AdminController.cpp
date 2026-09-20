#include "AdminController.h"

#include "../services/AdminService.h"

drogon::HttpResponsePtr AdminController::users(
    const drogon::HttpRequestPtr &)
{
    Json::Value result =
        AdminService::getUsers();

    return drogon::HttpResponse::newHttpJsonResponse(
        result);
}

drogon::HttpResponsePtr AdminController::orders(
    const drogon::HttpRequestPtr &)
{
    Json::Value result =
        AdminService::getOrders();

    return drogon::HttpResponse::newHttpJsonResponse(
        result);
}

drogon::HttpResponsePtr AdminController::deleteProduct(
    const drogon::HttpRequestPtr &,
    int productId)
{
    AdminService::deleteProduct(productId);

    Json::Value result;

    result["message"] =
        "Product removed by admin";

    return drogon::HttpResponse::newHttpJsonResponse(
        result);
}