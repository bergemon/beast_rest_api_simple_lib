#include "../include/dependencies.hpp"
#include "../include/server/server.hpp"
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
        // Allowed methods for this route
        { Method::GET, Method::HEAD },
        // Route target
        "/users",
        // Queries
        { {"min"}, {"max"} },
        // Route handler
        [&](const uint32_t version, const bool keep_alive) {
            http::response<http::string_body> res{http::status::bad_request, version};
            res.set(http::field::server, "Rest API by bergemon");
            res.set(http::field::content_type, "text/plain");
            res.keep_alive(keep_alive);
            res.body() = std::string("Hello!");
            res.prepare_payload();
            return res;
        }
    );

    return EXIT_SUCCESS;
}