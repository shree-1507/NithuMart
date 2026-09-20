#pragma once

#include <json/json.h>

namespace AuthService
{
    Json::Value registerUser(const Json::Value &data);
    Json::Value loginUser(const Json::Value &data);
}