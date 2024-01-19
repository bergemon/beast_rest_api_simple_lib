﻿#include "../include/dependencies.hpp"
#include "../include/server.hpp"
#include "include/variables.hpp"

#define _DEBUG

int main(int argc, char** argv) {

#ifdef _WIN32
#ifdef _DEBUG
    setlocale(LC_ALL, "Rus");
#endif
#endif

    std::cout << "Pet REST API by bergemon ver. "
        << APP_VERSION_MAJOR << '.' << APP_VERSION_MINOR << '\n'
        << "WebSocket listener will run on the passed port plus five."
        << std::endl;

    // Check command line arguments.
    if (argc < 2 && argc != 3) {
        std::cerr << "Usage: [AppName] <port> <threads>\n" << "Example: [AppName] 80 1\n"
            << "Passing number of threads is not necessary.\n"
            << "If you don't pass number of threads, there will be only one."
            << std::endl;
        return EXIT_FAILURE;
    }
    auto const port = static_cast<unsigned short>(std::atoi(argv[1]));
    int threads = 1;
    if (argc > 2)
        threads = std::max<int>(1, std::atoi(argv[2]));

    bergemon::Server server(port, threads);

    server.ROUTE(
        // Allowed methods<enum Method> for this route
        { GET, HEAD },
        // Route target<string>
        "/users",
        // Queries (query<string>, is_required<bool>, type<enum Type>)
        { {"min", true, INT}, {"max", true, INT} },
        // Route handler
        [&](std::string target, std::list<ParsedQuery> queries, Method method) {
            bergemon::Response res;
            res.body("Test response");
            res.bodyType(TEXT);

            return res;
        }
    );

    return EXIT_SUCCESS;
}