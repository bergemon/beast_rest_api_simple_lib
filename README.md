# Simple library to handle incoming requests - b_net

## Can be used not only for the Rest API - you can also set static files to the created routes

### Quick Start:

You can simply create a route like it's written below:

```
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
```

Beware of copying returning object. Return it to a reference.

**Arguments of the ROOT_ROUTE method are:**

> *std::vector containing b_net::Method objects*

> *route target, presented by std::string*

> *handler with signature **std::function<void>(b_net::Request&, b_net::Response)***

As you have seen handler does not returning any value. You need to set response fields and body by using b_net::Response methods.

### Then you can create a nested route aka sub route as written below.

This route will handle request to "/image/file/" target

```
auto& file_route = image_route.SUB_ROUTE(
    { POST },
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
```

**b_net::Request object have such fields:**
    *http request fields*
    *body and it's size in bytes*
    *cookies*
    *request method*
    *request queries*
    *request target*
    *body type - by conversing from http field "Content-Type" to b_net::BodyType enum value*
    *http request protocol version in uint32_t*