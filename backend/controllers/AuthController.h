#pragma once

#include <drogon/drogon.h>

class AuthController
{
public:
    static drogon::HttpResponsePtr registerUser(
        const drogon::HttpRequestPtr &request);

    static drogon::HttpResponsePtr login(
        const drogon::HttpRequestPtr &request);
};