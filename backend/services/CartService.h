#pragma once

#include <json/json.h>

namespace CartService
{
    Json::Value getCart(int buyerId);
    void addToCart(const Json::Value &data);
    void updateQuantity(int cartId, int quantity);
    void removeFromCart(int cartId);
}