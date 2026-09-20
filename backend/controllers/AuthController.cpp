#include "AuthController.h"
#include "../services/AuthService.h"

drogon::HttpResponsePtr AuthController::registerUser(
    const drogon::HttpRequestPtr &request)
{
    auto json = request->getJsonObject();

    if (!json)
    {
        Json::Value error;
        error["error"] = "JSON body is required";

        return drogon::HttpResponse::newHttpJsonResponse(error);
    }

    try
    {
        Json::Value result =
            AuthService::registerUser(*json);

        return drogon::HttpResponse::newHttpJsonResponse(result);
    }
    catch (const std::exception &e)
    {
        Json::Value error;
        error["error"] = e.what();

        auto response =
            drogon::HttpResponse::newHttpJsonResponse(error);

        response->setStatusCode(drogon::k400BadRequest);

        return response;
    }
}

drogon::HttpResponsePtr AuthController::login(
    const drogon::HttpRequestPtr &request)
{
    auto json = request->getJsonObject();

    if (!json)
    {
        Json::Value error;
        error["error"] = "JSON body is required";

        return drogon::HttpResponse::newHttpJsonResponse(error);
    }

    try
    {
        Json::Value result =
            AuthService::loginUser(*json);

        return drogon::HttpResponse::newHttpJsonResponse(result);
    }
    catch (const std::exception &e)
    {
        Json::Value error;
        error["error"] = e.what();

        auto response =
            drogon::HttpResponse::newHttpJsonResponse(error);

        response->setStatusCode(
            drogon::k401Unauthorized);

        return response;
    }
}