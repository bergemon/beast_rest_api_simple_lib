#pragma once
#include "error_response.hpp"

namespace b_net {
    http::message_generator handle_route(
        b_net::Request req,
        b_net::Response& res,
        const RouteHandler& route_handler
    )
    {
        // Check request method
        if(!route_handler.methodAllowed(req.method()))
        {
            return b_net_errs::bad_request(
                "Bad request. Method not allowed",
                req.version(),
                req.keep_alive(),
                req.method()
            );
        }

        // Check query parameters
        if(!route_handler.queriesExist(req.queries()))
        {
            return b_net_errs::bad_request(
                "Bad request. Not allowed query parameter(s)",
                req.version(),
                req.keep_alive(),
                req.method()
            );
        }

        // Invoke route handler and get custom b_net response class
        try
        {
            route_handler.handler()(req, res.clear());
        }
        catch (const std::exception& e)
        {
            return b_net_errs::static_file_not_found(req, e.what());
        }

        // Send response, we finally handle the request here
        return create_response(res, req.version(), req.keep_alive(), req.method());
    }
}