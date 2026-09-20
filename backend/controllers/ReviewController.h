#pragma once

#include <drogon/drogon.h>

class ReviewController
{
public:
    static drogon::HttpResponsePtr add(
        const drogon::HttpRequestPtr &request);

    static drogon::HttpResponsePtr get(
        const drogon::HttpRequestPtr &request,
        int productId);
};