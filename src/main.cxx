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
            b_net::Request& req,
            b_net::Response& res
        ) {
            b_net::error_code ec = res.file_body("image2.png");
            if (ec.get_status() != b_net::status::OK) {
                std::cerr << ec.message() << std::endl;
            }
        }
    );

    server.ROUTE(
        // Allowed methods<enum Method> for this route
        { ALL, GET, HEAD },
        // Route target<string>
        "/test",
        // Queries (query<string>, is_required<bool>, type<enum Type>)
        {
            {"min", false, INT_},
            {"max", false, INT_},
            {"test", false, INT_},
            {"hello", false, INT_}
        },
        // Route handler
        [&](
            b_net::Request& req,
            b_net::Response& res
        ) {
            std::stringstream ss;
            std::error_code ec;
            ss << "This is test binary response!\n";
            for (const auto& elem : req.queries()) {
                if (elem.query() != req.queries().back().query()) {
                    ss << elem.query() << ": " << elem.value() << '\n';
                    continue;
                }
                ss << elem.query() << ": " << elem.value();
            }
            for (const auto& elem : req.cookies()) {
                ss << elem.name() << ": " << elem.value() << '\n';
            }
            res.body(ss.str().c_str(), ss.str().length(), BINARY);
        }
    );

    server.run();

    return EXIT_SUCCESS;
}