#include "AuthService.h"

#include <drogon/drogon.h>

#include <stdexcept>
#include <string>

Json::Value AuthService::registerUser(
    const Json::Value &data)
{
    if (!data.isMember("name") ||
        !data.isMember("email") ||
        !data.isMember("password") ||
        !data.isMember("role"))
    {
        throw std::runtime_error(
            "All fields are required");
    }

    std::string name =
        data["name"].asString();

    std::string email =
        data["email"].asString();

    std::string password =
        data["password"].asString();

    std::string role =
        data["role"].asString();

    if (role != "Buyer" &&
        role != "Seller")
    {
        throw std::runtime_error(
            "Role must be Buyer or Seller");
    }

    auto db =
        drogon::app().getDbClient("default");

    auto existing =
        db->execSqlSync(
            "SELECT id FROM users WHERE email=$1",
            email
        );

    if (!existing.empty())
    {
        throw std::runtime_error(
            "Email already registered");
    }

    auto result =
        db->execSqlSync(
            "INSERT INTO users"
            "(name,email,password,role) "
            "VALUES($1,$2,$3,$4) "
            "RETURNING id,name,email,role",
            name,
            email,
            password,
            role
        );

    Json::Value user;

    user["id"] =
        result[0]["id"].as<int>();

    user["name"] =
        result[0]["name"].as<std::string>();

    user["email"] =
        result[0]["email"].as<std::string>();

    user["role"] =
        result[0]["role"].as<std::string>();

    return user;
}

Json::Value AuthService::loginUser(
    const Json::Value &data)
{
    if (!data.isMember("email") ||
        !data.isMember("password"))
    {
        throw std::runtime_error(
            "Email and password are required");
    }

    std::string email =
        data["email"].asString();

    std::string password =
        data["password"].asString();

    auto db =
        drogon::app().getDbClient("default");

    auto result =
        db->execSqlSync(
            "SELECT id,name,email,password,role "
            "FROM users WHERE email=$1",
            email
        );

    if (result.empty())
    {
        throw std::runtime_error(
            "Invalid email or password");
    }

    if (result[0]["password"].as<std::string>()
        != password)
    {
        throw std::runtime_error(
            "Invalid email or password");
    }

    Json::Value user;

    user["id"] =
        result[0]["id"].as<int>();

    user["name"] =
        result[0]["name"].as<std::string>();

    user["email"] =
        result[0]["email"].as<std::string>();

    user["role"] =
        result[0]["role"].as<std::string>();

    user["message"] =
        "Login successful";

    return user;
}