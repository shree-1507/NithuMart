#pragma once

#include <drogon/drogon.h>

class AdminController
{
public:
    static drogon::HttpResponsePtr users(
        const drogon::HttpRequestPtr &request);

    static drogon::HttpResponsePtr orders(
        const drogon::HttpRequestPtr &request);

    static drogon::HttpResponsePtr deleteProduct(
        const drogon::HttpRequestPtr &request,
        int productId);
};