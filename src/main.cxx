#include "../include/dependencies.hpp"
#include "include/variables.hpp"

// routes
#include "../include/routes.hpp"

int main(int argc, char** argv) {
#ifdef _WIN32
    setlocale(LC_ALL, "Rus");
#endif

    std::cout << "Pet REST API by bergemon ver. "
        << APP_VERSION_MAJOR << '.' << APP_VERSION_MINOR << '\n'
        << "WebSocket listener will run on the passed port plus five."
        << std::endl;

    auto& o1 = server.ROOT_ROUTE(
        // Allowed methods<enum Method> for this route
        { GET, HEAD },
        // Route target<string>
        "/users",
        // Queries (query<string>, is_required<bool>, type<enum Type>)
        { {"min", true, INT_}, {"max", true, INT_} },
        // Route handler
        getUsers
    );

    auto& o2 = server.ROOT_ROUTE(
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

    auto& o3 = server.ROOT_ROUTE(
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
            ss << "Cookies: \n";
            for (const auto& elem : req.queries()) {
                if (elem.name() != req.queries().back().name()) {
                    ss << elem.name() << ": " << elem.value() << '\n';
                    continue;
                }
                ss << elem.name() << ": " << elem.value();
            }
            for (const auto& elem : req.cookies()) {
                ss << elem.name() << ": " << elem.value() << '\n';
            }
            res.body(ss.str().c_str(), ss.str().length(), TEXT);
        }
    );

    o3.ROUTE(
        // Allowed methods<enum Method> for this route
        { GET },
        // Route target<string>
        "/file",
        // Route handler
        [&](
            b_net::Request& req,
            b_net::Response& res
        ) {
            std::string file_name = "file";
            file_name += req.mime_type();
            std::ofstream file(file_name, std::ios::binary | std::ios::out);
            if (file.is_open())
            {
                file.write(req.body(), req.body_size());
            }

            res.body("{\"status\": \"ok\"}", JSON);
        }
    );

    server.run();

    return EXIT_SUCCESS;
}