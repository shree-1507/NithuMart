#include <iostream>
#include <drogon/drogon.h>

#include "database/Database.h"
#include "routes/Routes.h"

int main()
{
    std::cerr << "STEP 1: Program started\n";

    std::cerr << "STEP 2: Initializing database...\n";
    Database::initialize();
    std::cerr << "STEP 3: Database initialized\n";

    std::cerr << "STEP 4: Registering routes...\n";
    Routes::registerAll();
    std::cerr << "STEP 5: Routes registered\n";

    std::cerr << "STEP 6: Setting document root...\n";
    drogon::app().setDocumentRoot("C:/Dev/NithuMart/frontend");
    std::cerr << "STEP 7: Document root set\n";

    std::cerr << "STEP 8: Starting server...\n";

    drogon::app()
        .setThreadNum(4)
        .addListener("127.0.0.1", 8080)
        .run();

    std::cerr << "STEP 9: Server stopped\n";

    return 0;
}