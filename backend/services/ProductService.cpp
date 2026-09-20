#include "ProductService.h"

#include <drogon/drogon.h>

#include <stdexcept>
#include <string>

namespace ProductService
{
    Json::Value getAllProducts()
    {
        auto db = drogon::app().getDbClient("default");

        auto result = db->execSqlSync(
            "SELECT id,seller_id,name,description,"
            "price,category,stock "
            "FROM products ORDER BY id DESC"
        );

        Json::Value products(Json::arrayValue);

        for (const auto &row : result)
        {
            Json::Value product;

            product["id"] = row["id"].as<int>();
            product["seller_id"] =
                row["seller_id"].as<int>();

            product["name"] =
                row["name"].as<std::string>();

            product["description"] =
                row["description"].isNull()
                ? ""
                : row["description"].as<std::string>();

            product["price"] =
                row["price"].as<double>();

            product["category"] =
                row["category"].isNull()
                ? ""
                : row["category"].as<std::string>();

            product["stock"] =
                row["stock"].as<int>();

            products.append(product);
        }

        return products;
    }

    Json::Value getProduct(int id)
    {
        auto db = drogon::app().getDbClient("default");

        auto result = db->execSqlSync(
            "SELECT id,seller_id,name,description,"
            "price,category,stock "
            "FROM products WHERE id=$1",
            id
        );

        if (result.empty())
        {
            throw std::runtime_error(
                "Product not found");
        }

        Json::Value product;

        product["id"] = result[0]["id"].as<int>();
        product["seller_id"] =
            result[0]["seller_id"].as<int>();

        product["name"] =
            result[0]["name"].as<std::string>();

        product["description"] =
            result[0]["description"].isNull()
            ? ""
            : result[0]["description"].as<std::string>();

        product["price"] =
            result[0]["price"].as<double>();

        product["category"] =
            result[0]["category"].isNull()
            ? ""
            : result[0]["category"].as<std::string>();

        product["stock"] =
            result[0]["stock"].as<int>();

        return product;
    }

    Json::Value addProduct(const Json::Value &data)
    {
        if (!data.isMember("seller_id") ||
            !data.isMember("name") ||
            !data.isMember("price") ||
            !data.isMember("stock"))
        {
            throw std::runtime_error(
                "Required product fields are missing");
        }

        int sellerId = data["seller_id"].asInt();
        std::string name = data["name"].asString();
        std::string description =
            data.get("description", "").asString();

        double price = data["price"].asDouble();

        std::string category =
            data.get("category", "").asString();

        int stock = data["stock"].asInt();

        if (price < 0 || stock < 0)
        {
            throw std::runtime_error(
                "Price and stock cannot be negative");
        }

        auto db = drogon::app().getDbClient("default");

        auto result = db->execSqlSync(
            "INSERT INTO products"
            "(seller_id,name,description,price,category,stock) "
            "VALUES($1,$2,$3,$4,$5,$6) "
            "RETURNING id",
            sellerId,
            name,
            description,
            price,
            category,
            stock
        );

        Json::Value product;

        product["id"] =
            result[0]["id"].as<int>();

        product["message"] =
            "Product added successfully";

        return product;
    }

    Json::Value updateProduct(
        int id,
        const Json::Value &data)
    {
        if (!data.isMember("name") ||
            !data.isMember("price") ||
            !data.isMember("stock"))
        {
            throw std::runtime_error(
                "Required fields are missing");
        }

        std::string name =
            data["name"].asString();

        std::string description =
            data.get("description", "").asString();

        double price =
            data["price"].asDouble();

        std::string category =
            data.get("category", "").asString();

        int stock =
            data["stock"].asInt();

        auto db = drogon::app().getDbClient("default");

        auto result = db->execSqlSync(
            "UPDATE products SET "
            "name=$1,description=$2,price=$3,"
            "category=$4,stock=$5 "
            "WHERE id=$6",
            name,
            description,
            price,
            category,
            stock,
            id
        );

        if (result.affectedRows() == 0)
        {
            throw std::runtime_error(
                "Product not found");
        }

        Json::Value response;

        response["message"] =
            "Product updated successfully";

        return response;
    }

    void deleteProduct(int id)
    {
        auto db = drogon::app().getDbClient("default");

        auto result = db->execSqlSync(
            "DELETE FROM products WHERE id=$1",
            id
        );

        if (result.affectedRows() == 0)
        {
            throw std::runtime_error(
                "Product not found");
        }
    }
}