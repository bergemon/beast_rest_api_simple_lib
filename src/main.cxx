#include "../include/dependencies.hpp"
#include "../simple_server/server.hpp"
#include "include/variables.hpp"

// routes
#include "../include/routes.hpp"

#define _DEBUG

int main(int argc, char** argv) {

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

    b_net::Server server(port, threads);

    server.ROUTE(
        // Allowed methods<enum Method> for this route
        { GET, HEAD },
        // Route target<string>
        "/users",
        // Queries (query<string>, is_required<bool>, type<enum Type>)
        { {"min", true, INT_}, {"max", true, INT_} },
        // Route handler
        getUsers
    );

    server.ROUTE(
        // Allowed methods<enum Method> for this route
        { GET, HEAD },
        // Route target<string>
        "/image",
        // Route handler
        [&](
            b_net::Response& res,
            std::string target,
            std::list<b_net::ParsedQuery>& queries,
            b_net::Method method
        ) {
            std::ifstream file("image.jpg", std::ios::ate | std::ios::binary);
            std::streamsize size = 0;
            char* buffer = nullptr;

            if (file.is_open()) {
                file.seekg( 0, std::ios::end );
                size = file.tellg();
                file.seekg( 0, std::ios::beg );
                if (size > 0) {
                    buffer = new char[size];
                    file.read(buffer, size);
                }
            }
            file.close();

            res.body(buffer, size, BINARY);
        }
    );

    server.ROUTE(
        // Allowed methods<enum Method> for this route
        { GET, HEAD },
        // Route target<string>
        "/test",
        // Queries (query<string>, is_required<bool>, type<enum Type>)
        { {"min", true, INT_}, {"max", true, INT_} },
        // Route handler
        [&](
            b_net::Response& res,
            std::string target,
            std::list<b_net::ParsedQuery>& queries,
            b_net::Method method
        ) {
            std::stringstream ss;
            int min, max;

            for (const auto& elem : queries) {
                if (elem.query() == "min")
                    min = std::atoi(elem.value().c_str());
                if (elem.query() == "max")
                    max = std::atoi(elem.value().c_str());
            }

            ss << "This is test binary response!\n"
                << "Min: " << min
                << ", Max: " << max << std::endl;

            res.body(ss.str().c_str(), ss.str().length(), BINARY);
        }
    );

    server.run();

    return EXIT_SUCCESS;
}