
#include "Database.h"

#include <drogon/drogon.h>
#include <drogon/orm/DbConfig.h>

#include <cstdlib>
#include <stdexcept>
#include <string>

static std::string getEnv(const char* name)
{
    const char* value = std::getenv(name);

    if (value == nullptr || std::string(value).empty())
    {
        throw std::runtime_error(
            std::string(name) + " is not set");
    }

    return value;
}

void Database::initialize()
{
    drogon::orm::PostgresConfig config;

    config.host = getEnv("NITHUMART_DB_HOST");
    config.port = std::stoi(getEnv("NITHUMART_DB_PORT"));
    config.databaseName = getEnv("NITHUMART_DB_NAME");
    config.username = getEnv("NITHUMART_DB_USER");
    config.password = getEnv("NITHUMART_DB_PASSWORD");

    config.connectionNumber = 4;
    config.name = "default";
    config.isFast = false;
    config.timeout = -1;
    config.autoBatch = false;

    drogon::app().addDbClient(config);
}