#pragma once

#include <string>

struct Review
{
    int id;
    int buyerId;
    int productId;
    int rating;
    std::string comment;
};