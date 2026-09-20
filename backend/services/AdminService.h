#pragma once

#include <json/json.h>

namespace AdminService
{
    Json::Value getUsers();
    Json::Value getOrders();
    void deleteProduct(int productId);
}