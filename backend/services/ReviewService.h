#pragma once

#include <json/json.h>

namespace ReviewService
{
    Json::Value addReview(const Json::Value &data);
    Json::Value getReviews(int productId);
}