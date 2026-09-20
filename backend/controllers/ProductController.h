#pragma once

#include <drogon/drogon.h>

class ProductController
{
public:
    static drogon::HttpResponsePtr getAll(
        const drogon::HttpRequestPtr &request);

    static drogon::HttpResponsePtr getOne(
        const drogon::HttpRequestPtr &request,
        int id);

    static drogon::HttpResponsePtr add(
        const drogon::HttpRequestPtr &request);

    static drogon::HttpResponsePtr update(
        const drogon::HttpRequestPtr &request,
        int id);

    static drogon::HttpResponsePtr remove(
        const drogon::HttpRequestPtr &request,
        int id);
};