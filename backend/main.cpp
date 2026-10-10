#include <iostream>
#include <drogon/drogon.h>

#include "database/Database.h"
#include "routes/Routes.h"

#include <cstdlib>
#include <string>

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
    drogon::app().setDocumentRoot("/app/frontend");
    std::cerr << "STEP 7: Document root set\n";

    std::cerr << "STEP 8: Starting server...\n";

    const char* portEnv = std::getenv("PORT");
int port = portEnv ? std::stoi(portEnv) : 10000;

drogon::app()
    .setThreadNum(4)
    .addListener("0.0.0.0", port)
    .run();

    std::cerr << "STEP 9: Server stopped\n";

    return 0;
}