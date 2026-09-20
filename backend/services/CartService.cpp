#include "CartService.h"

#include <drogon/drogon.h>

#include <stdexcept>

namespace CartService
{
    Json::Value getCart(int buyerId)
    {
        auto db =
            drogon::app().getDbClient("default");

        auto result =
            db->execSqlSync(
                "SELECT c.id, c.product_id, "
                "p.name, p.price, c.quantity, "
                "(p.price * c.quantity) AS subtotal "
                "FROM cart c "
                "JOIN products p "
                "ON p.id = c.product_id "
                "WHERE c.buyer_id = $1 "
                "ORDER BY c.id",
                buyerId
            );

        Json::Value items(Json::arrayValue);

        double total = 0.0;

        for (const auto &row : result)
        {
            Json::Value item;

            item["id"] =
                row["id"].as<int>();

            item["product_id"] =
                row["product_id"].as<int>();

            item["name"] =
                row["name"].as<std::string>();

            item["price"] =
                row["price"].as<double>();

            item["quantity"] =
                row["quantity"].as<int>();

            item["subtotal"] =
                row["subtotal"].as<double>();

            total +=
                row["subtotal"].as<double>();

            items.append(item);
        }

        Json::Value cart;

        cart["buyer_id"] = buyerId;
        cart["items"] = items;
        cart["total_price"] = total;

        return cart;
    }

    void addToCart(const Json::Value &data)
    {
        if (!data.isMember("buyer_id") ||
            !data.isMember("product_id") ||
            !data.isMember("quantity"))
        {
            throw std::runtime_error(
                "buyer_id, product_id and quantity are required");
        }

        int buyerId =
            data["buyer_id"].asInt();

        int productId =
            data["product_id"].asInt();

        int quantity =
            data["quantity"].asInt();

        if (quantity <= 0)
        {
            throw std::runtime_error(
                "Quantity must be greater than zero");
        }

        auto db =
            drogon::app().getDbClient("default");

        auto product =
            db->execSqlSync(
                "SELECT stock "
                "FROM products "
                "WHERE id = $1",
                productId
            );

        if (product.empty())
        {
            throw std::runtime_error(
                "Product not found");
        }

        if (quantity >
            product[0]["stock"].as<int>())
        {
            throw std::runtime_error(
                "Not enough stock");
        }

        db->execSqlSync(
            "INSERT INTO cart"
            "(buyer_id, product_id, quantity) "
            "VALUES($1, $2, $3) "
            "ON CONFLICT(buyer_id, product_id) "
            "DO UPDATE SET "
            "quantity = cart.quantity + EXCLUDED.quantity",
            buyerId,
            productId,
            quantity
        );
    }

    void updateQuantity(
        int cartId,
        int quantity)
    {
        if (quantity <= 0)
        {
            throw std::runtime_error(
                "Quantity must be greater than zero");
        }

        auto db =
            drogon::app().getDbClient("default");

        auto result =
            db->execSqlSync(
                "UPDATE cart "
                "SET quantity = $1 "
                "WHERE id = $2",
                quantity,
                cartId
            );

        if (result.affectedRows() == 0)
        {
            throw std::runtime_error(
                "Cart item not found");
        }
    }

    void removeFromCart(int cartId)
    {
        auto db =
            drogon::app().getDbClient("default");

        auto result =
            db->execSqlSync(
                "DELETE FROM cart "
                "WHERE id = $1",
                cartId
            );

        if (result.affectedRows() == 0)
        {
            throw std::runtime_error(
                "Cart item not found");
        }
    }
}