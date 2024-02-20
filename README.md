<h1 align="center" style="font-size: 40px">
    Simple library to handle incoming request - b_net
<h1>

<h2 align="center" style="font-size: 25px; font-family: ">
    Can be used not only for the Rest API - you can also set static files to the created routes
</h2>

<div style="display: flex; gap: 10px; flex-diration: column">
    <h3 align="center" style="font-size: 15px">Quick Start:</h3>
    <p style="font-size: 10px">
        You can simply create a route like it's written below
    </p>
    `\#include "simple_server/server.hpp"

    b_net::Server server;

    auto& image_route_1 = server.ROOT_ROUTE(
        // Allowed methods<enum Method> for this route
        { GET, HEAD },
        // Route target<string>
        "/image",
        // Route handler
        [&](
            b_net::Request& req,
            b_net::Response& res
        ) {
            b_net::error_code ec = res.file_body("image.jpg");
            if (ec.get_status() != b_net::status::OK) {
                std::cerr << ec.message() << std::endl;
            }
        }
    );`
</div>