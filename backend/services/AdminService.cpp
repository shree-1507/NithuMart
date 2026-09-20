#include "AdminService.h"

#include <drogon/drogon.h>

namespace AdminService
{
    Json::Value getUsers()
    {
        auto db =
            drogon::app().getDbClient("default");

        auto result =
            db->execSqlSync(
                "SELECT id,name,email,role "
                "FROM users ORDER BY id"
            );

        Json::Value users(Json::arrayValue);

        for (const auto &row : result)
        {
            Json::Value user;

            user["id"] =
                row["id"].as<int>();

            user["name"] =
                row["name"].as<std::string>();

            user["email"] =
                row["email"].as<std::string>();

            user["role"] =
                row["role"].as<std::string>();

            users.append(user);
        }

        return users;
    }

    Json::Value getOrders()
    {
        auto db =
            drogon::app().getDbClient("default");

        auto result =
            db->execSqlSync(
                "SELECT id,buyer_id,"
                "total_amount,status,order_date "
                "FROM orders "
                "ORDER BY id DESC"
            );

        Json::Value orders(Json::arrayValue);

        for (const auto &row : result)
        {
            Json::Value order;

            order["id"] =
                row["id"].as<int>();

            order["buyer_id"] =
                row["buyer_id"].as<int>();

            order["total_amount"] =
                row["total_amount"].as<double>();

            order["status"] =
                row["status"].as<std::string>();

            order["order_date"] =
                row["order_date"].as<std::string>();

            orders.append(order);
        }

        return orders;
    }

    void deleteProduct(int productId)
    {
        auto db =
            drogon::app().getDbClient("default");

        db->execSqlSync(
            "DELETE FROM products "
            "WHERE id=$1",
            productId
        );
    }
}