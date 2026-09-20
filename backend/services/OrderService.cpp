#include "OrderService.h"

#include <drogon/drogon.h>

#include <stdexcept>
#include <string>

namespace OrderService
{
    Json::Value placeOrder(int buyerId)
    {
        auto db =
            drogon::app().getDbClient("default");

        auto cart =
            db->execSqlSync(
                "SELECT c.product_id,"
                "c.quantity,p.price,p.stock "
                "FROM cart c "
                "JOIN products p "
                "ON p.id=c.product_id "
                "WHERE c.buyer_id=$1",
                buyerId
            );

        if (cart.empty())
        {
            throw std::runtime_error(
                "Cart is empty");
        }

        double total = 0;

        for (const auto &row : cart)
        {
            if (row["quantity"].as<int>() >
                row["stock"].as<int>())
            {
                throw std::runtime_error(
                    "Not enough stock");
            }

            total +=
                row["price"].as<double>() *
                row["quantity"].as<int>();
        }

        auto order =
            db->execSqlSync(
                "INSERT INTO orders"
                "(buyer_id,total_amount,status) "
                "VALUES($1,$2,'Pending') "
                "RETURNING id",
                buyerId,
                total
            );

        int orderId =
            order[0]["id"].as<int>();

        for (const auto &row : cart)
        {
            db->execSqlSync(
                "INSERT INTO order_items"
                "(order_id,product_id,quantity,price) "
                "VALUES($1,$2,$3,$4)",
                orderId,
                row["product_id"].as<int>(),
                row["quantity"].as<int>(),
                row["price"].as<double>()
            );

            db->execSqlSync(
                "UPDATE products "
                "SET stock=stock-$1 "
                "WHERE id=$2",
                row["quantity"].as<int>(),
                row["product_id"].as<int>()
            );
        }

        db->execSqlSync(
            "DELETE FROM cart WHERE buyer_id=$1",
            buyerId
        );

        Json::Value response;

        response["order_id"] = orderId;
        response["total_amount"] = total;
        response["status"] = "Pending";
        response["message"] =
            "Order placed successfully";

        return response;
    }

    Json::Value getBuyerOrders(int buyerId)
    {
        auto db =
            drogon::app().getDbClient("default");

        auto result =
            db->execSqlSync(
                "SELECT id,total_amount,"
                "status,order_date "
                "FROM orders "
                "WHERE buyer_id=$1 "
                "ORDER BY id DESC",
                buyerId
            );

        Json::Value orders(Json::arrayValue);

        for (const auto &row : result)
        {
            Json::Value order;

            order["id"] =
                row["id"].as<int>();

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

    Json::Value getSellerOrders(int sellerId)
    {
        auto db =
            drogon::app().getDbClient("default");

        auto result =
            db->execSqlSync(
                "SELECT DISTINCT "
                "o.id,o.buyer_id,"
                "o.total_amount,o.status,"
                "o.order_date "
                "FROM orders o "
                "JOIN order_items oi "
                "ON oi.order_id=o.id "
                "JOIN products p "
                "ON p.id=oi.product_id "
                "WHERE p.seller_id=$1 "
                "ORDER BY o.id DESC",
                sellerId
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

    void updateStatus(
        int orderId,
        const std::string &status)
    {
        if (status != "Pending" &&
            status != "Confirmed" &&
            status != "Delivered" &&
            status != "Cancelled")
        {
            throw std::runtime_error(
                "Invalid order status");
        }

        auto db =
            drogon::app().getDbClient("default");

        db->execSqlSync(
            "UPDATE orders SET status=$1 "
            "WHERE id=$2",
            status,
            orderId
        );
    }
}