# Simple library to handle incoming requests - b_net

## Can be used not only for the Rest API - you can also set static files to the created routes

> [!IMPORTANT]
> Library is not completed yet. The only directory you need to write a server is a simple_server catalog.

> [!NOTE]
> This library uses boost.asio and boost.beast as a dependencies!

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

> [!WARNING]
> Beware of copying returning object. Return it to a reference. Boost Beast library, that used as a dependency, prohibit to delete copying constructors. And i don't know how to bypass it for now.

**Arguments of the ROOT_ROUTE method are:**

+ *std::vector containing b_net::Method objects*
+ *route target, presented by std::string*
+ *route queries in std::vector, each has it's name and value*
+ *handler with signature **std::function<void>(b_net::Request&, b_net::Response)***

As you have seen handler does not returning any value. You need to set response fields and body by using b_net::Response methods.

### Then you can create a nested route aka sub route as written below:

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

### Route queries

You can set queries to the route and set is it required or not. You don't need to insert queries into the method when creating the route - it's not necessary.
In future releases you will be able to set queries value types. It is uselees to set query value type for now but it's not necessary.

> [!IMPORTANT]
> If you don't insert any queries in route creator methods, then your route will not have any queries and client can't request target that will contain any queries!

Here is an example how to create a route with queries, how to get the request queries and handle them:
```
#include "simple_server/server.hpp"

b_net::Server server;

auto& users_route = server.ROOT_ROUTE(
    { GET, HEAD },
    "/users",
    { {"min", true, INT_}, {"max", true, INT_} },
    [&](
        b_net::Request& req,
        b_net::Response& res
    ) {
        int min, max;
        for (const auto& elem : req.queries()) {
            if (elem.name() == "min") {
                min = std::atoi(elem.value().c_str());
            }
            if (elem.name() == "max") {
                max = std::atoi(elem.value().c_str());
            }
        }

        std::stringstream ss;

        ss << "Test /users response!\n"
            << "Query parameters:\n"
            << "min = " << min << '\n'
            << "max = " << max;

        res.body(ss.str());
    }
);
```

**b_net::Request object have such fields:**
+ *http request fields*
+ *body and it's size in bytes*
+ *cookies*
+ *request method*
+ *request queries*
+ *request target*
+ *body type - by conversing from http field "Content-Type" to b_net::BodyType enum value*
+ *http request protocol version in uint32_t*
+ *keep alive field in a boolean value*

### The handler

Handler has two parameters - b_net::Request& and b_net::Response& object. This library will insert those objects into the handler, where you can get some request data from b_net::Request object and create response by setting new http protocol fields and body to it. Library will compute body size in octets by itself. It also can compute your file extension in response and type of the body in request, presented in b_net::BodyType by conversing from "Content-Type" field, then you can easily handle request body type in handler function.

### Nesting

Library hasn't any limits to route nesting. You can have any level of nesting routes. Library provides easily customized routes for you as developer.

### TLS (https)

Library doesn't provide HTTPS requests and responses. It needs a proxy, like nginx for example if you want to use TLS.

### Static files

You can set static files to the route if it returns an html file and the browser will make new requests to get them. Here is an example how to do it:

```
#include "simple_server/server.hpp"

b_net::Server server;

const std::string website_catalog{ "website/" };

auto& index_route = server.ROOT_ROUTE(
    { GET, HEAD },
    "/",
    [&](
        b_net::Request& req,
        b_net::Response& res
    ) {
        b_net::error_code ec = res.file_body(website_catalog + "index.html");

        if(ec.error())
            std::cout << ec.message() << std::endl;
    }
);

index_route.static_file(website_catalog + "main.css");
```

### Slash at the end of the request target

This library will redirrect any request if it does not have a slash at the end of the requested target. For example - if requested route is /test_route, then library will redirrect request to /test_route/ target with code 301 (Moved permanently).