#include "Database.h"

#include <drogon/drogon.h>
#include <drogon/orm/DbConfig.h>

#include <cstdlib>
#include <stdexcept>
#include <string>

void Database::initialize()
{
    const char *password =
        std::getenv("NITHUMART_DB_PASSWORD");

    if (password == nullptr ||
        std::string(password).empty())
    {
        throw std::runtime_error(
            "NITHUMART_DB_PASSWORD is not set");
    }

    drogon::orm::PostgresConfig config;

    config.host = "127.0.0.1";
    config.port = 5432;
    config.databaseName = "NithuMart";
    config.username = "postgres";
    config.password = password;

    config.connectionNumber = 4;
    config.name = "default";
    config.isFast = false;
    config.timeout = -1;
    config.autoBatch = false;

    drogon::app().addDbClient(config);
}