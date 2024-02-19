#pragma once
#include "error_response.hpp"

namespace b_net {
    http::message_generator handle_route(
        b_net::Request req,
        b_net::Response& res,
        const RouteHandler& route
    )
    {
        // Check request method
        if(!route.methodAllowed(req.method()))
        {
            return b_net_errs::bad_request(
                "Bad request. Method not allowed",
                req
            );
        }

        // Check query parameters
        if(!route.queriesExist(req.queries()))
        {
            return b_net_errs::bad_request(
                "Bad request. Not allowed query parameter(s)",
                req
            );
        }

        // Invoke route handler and get custom b_net response class
        route.handler()(req, res.clear());

        // Send response, we finally handle the request here
        return create_response(res, req.version(), req.keep_alive(), req.method());
    }
}