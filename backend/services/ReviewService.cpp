#include "ReviewService.h"

#include <drogon/drogon.h>

#include <stdexcept>

namespace ReviewService
{
    Json::Value addReview(const Json::Value &data)
    {
        if (!data.isMember("buyer_id") ||
            !data.isMember("product_id") ||
            !data.isMember("rating"))
        {
            throw std::runtime_error(
                "Required review fields are missing");
        }

        int buyerId =
            data["buyer_id"].asInt();

        int productId =
            data["product_id"].asInt();

        int rating =
            data["rating"].asInt();

        std::string comment =
            data.get("comment", "").asString();

        if (rating < 1 || rating > 5)
        {
            throw std::runtime_error(
                "Rating must be between 1 and 5");
        }

        auto db =
            drogon::app().getDbClient("default");

        db->execSqlSync(
            "INSERT INTO reviews"
            "(buyer_id,product_id,rating,comment) "
            "VALUES($1,$2,$3,$4) "
            "ON CONFLICT(buyer_id,product_id) "
            "DO UPDATE SET "
            "rating=EXCLUDED.rating,"
            "comment=EXCLUDED.comment",
            buyerId,
            productId,
            rating,
            comment
        );

        Json::Value response;

        response["message"] =
            "Review added successfully";

        return response;
    }

    Json::Value getReviews(int productId)
    {
        auto db =
            drogon::app().getDbClient("default");

        auto result =
            db->execSqlSync(
                "SELECT r.id,r.buyer_id,"
                "u.name,r.rating,r.comment "
                "FROM reviews r "
                "JOIN users u "
                "ON u.id=r.buyer_id "
                "WHERE r.product_id=$1 "
                "ORDER BY r.id DESC",
                productId
            );

        Json::Value reviews(Json::arrayValue);

        for (const auto &row : result)
        {
            Json::Value review;

            review["id"] =
                row["id"].as<int>();

            review["buyer_id"] =
                row["buyer_id"].as<int>();

            review["buyer_name"] =
                row["name"].as<std::string>();

            review["rating"] =
                row["rating"].as<int>();

            review["comment"] =
                row["comment"].isNull()
                ? ""
                : row["comment"].as<std::string>();

            reviews.append(review);
        }

        return reviews;
    }
}