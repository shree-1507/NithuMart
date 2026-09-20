#pragma once

#include <json/json.h>

namespace ProductService
{
    Json::Value getAllProducts();
    Json::Value getProduct(int id);
    Json::Value addProduct(const Json::Value &data);
    Json::Value updateProduct(int id, const Json::Value &data);
    void deleteProduct(int id);
}