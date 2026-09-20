#pragma once

#include <string>

struct Product
{
    int id;
    int sellerId;
    std::string name;
    std::string description;
    double price;
    std::string category;
    int stock;
};