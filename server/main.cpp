#include "server.hpp"
#include "database/database.hpp"

#include <iostream>

#include <asio.hpp>

int main() {

    try {
        asio::io_context ioContext;

        Database database{};
        Server server(ioContext, database);

        ioContext.run();
    }

    catch (std::exception &e) {
        std::cerr << e.what() << "\n";
    }

    return 0;
}
