#pragma once

#include <string>

struct Order
{
    int id;
    int buyerId;
    double totalAmount;
    std::string status;
};