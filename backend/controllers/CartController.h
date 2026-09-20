#pragma once

#include <drogon/drogon.h>

class CartController
{
public:
    static drogon::HttpResponsePtr get(
        const drogon::HttpRequestPtr &request,
        int buyerId);

    static drogon::HttpResponsePtr add(
        const drogon::HttpRequestPtr &request);

    static drogon::HttpResponsePtr update(
        const drogon::HttpRequestPtr &request,
        int cartId);

    static drogon::HttpResponsePtr remove(
        const drogon::HttpRequestPtr &request,
        int cartId);
};