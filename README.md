<h1 align="center" style="font-size: 40px">
    Simple library to handle incoming request - b_net
<h1>

<h2 align="center" style="font-size: 25px; font-family: ">
    Can be used not only for the Rest API - you can also set static files to the created routes
</h2>

<div style="display: flex; gap: 10px; flex-diration: column">
<h3 align="center" style="font-size: 15px">Quick Start:</h3>
<p style="font-size: 10px">
    You can simply create a route like it's written below:
</p>

        #include "simple_server/server.hpp"

        b_net::Server server;

        auto& image_route = server.ROOT_ROUTE(
            { GET, HEAD },
            "/image",
            [&](
                b_net::Request& req,
                b_net::Response& res
            ) {
                b_net::error_code ec = res.file_body("image.jpg");
                if (ec.get_status() != b_net::status::OK) {
                    std::cerr << ec.message() << std::endl;
                }
            }
        );

<p style="font-size: 10px">
    Beware of copying returning object. Return it to a reference.
    Arguments of the ROOT_ROUTE method are:
    <div style="display: flex; flex-direction: column; gap: 5px">
        <span>std::vector containing b_net::Method objects</span>
        <span>route target, presented by std::string</span>
        <span>handler with signature std::function<void>(b_net::Requst&, b_net::Response)</span>
    </div>

    As you have seen handler does not returning any value. You need to set
    response fields and body by using b_net::Response methods.
</p>
<p style="font-size: 10px">
    Then you can create a nested route aka sub route as written below.
    This route will handle request to "/image/file/" target
</p>

        auto& avatar_image = image_route.SUB_ROUTE(
            { GET },
            "/file",
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
</div>