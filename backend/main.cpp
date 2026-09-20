#include <drogon/drogon.h>

#include "database/Database.h"
#include "routes/Routes.h"

int main()
{
    Database::initialize();
    Routes::registerAll();

    drogon::app()
        .setThreadNum(4)
        .addListener("127.0.0.1", 8080)
        .run();

    return 0;
}