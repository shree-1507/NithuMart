#include "ReviewController.h"

#include "../services/ReviewService.h"

drogon::HttpResponsePtr ReviewController::add(
    const drogon::HttpRequestPtr &request)
{
    auto json = request->getJsonObject();

    try
    {
        Json::Value result =
            ReviewService::addReview(*json);

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

drogon::HttpResponsePtr ReviewController::get(
    const drogon::HttpRequestPtr &,
    int productId)
{
    Json::Value result =
        ReviewService::getReviews(productId);

    return drogon::HttpResponse::newHttpJsonResponse(
        result);
}