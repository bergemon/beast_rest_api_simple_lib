#pragma once
#include "errorResponse.hpp"

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request (
        b_net::Response& res,
        http::request<Body, http::basic_fields<Allocator>>&& req,
        std::vector<b_net::RootRoute>& root_routes
    )
    {
        /////////////////////////////////////////////////
        // Initializing of custom b_net request class //
        ////////////////////////////////////////////////

        // Declare variables here to prevent copying of them
        const std::string target = req.target();
        std::list<ParsedField> queries;
        // Declare and get header fields
        std::list<ParsedField> header_fields;
        utility_::get_req_header_fields(req.base(), header_fields);
        // Declare and parse cookies
        std::list<ParsedField> cookies;

        // Construct custom b_net request class for the handler
        b_net::Request req_(
            // Request target, version and keep alive field
            target, req.version(), req.keep_alive(),
            // Request body and it's size
            req.body(), req.body().size(),
            // Get http header fields
            header_fields,
            // Parse and pass list of queries
            utility_::parse_queries(target, queries),
            // Parse and pass list of cookies
            utility_::parse_cookies(header_fields, cookies),
            // Convert beast method to b_net method
            utility_::convert_method(req.method()),
            // Parse type of incoming file - how to decode octets
            utility_::mimeType_to_bodyType(utility_::parse_bodyType(req.base()))
        );

        // Check target - is it valid or not
        if (!utility_::isValid(target))
            return b_net_errs::bad_request(
                "Bad request. Illegal request-target",
                req_
            );

        // Handle incoming request
        for (auto& rr : root_routes) {
            using namespace b_net;

            // Check is target contains root route
            // Handle request if it's target equal to root route
            if(rr.route().isTarget(target))
            {
                // Check request method
                if(!rr.route().methodAllowed(req.method()))
                {
                    return b_net_errs::bad_request(
                        "Bad request. Method not allowed",
                        req_
                    );
                }

                // Check query parameters
                if(!rr.route().queriesExist(req_.queries()))
                {
                    return b_net_errs::bad_request(
                        "Bad request. Not allowed query parameter(s)",
                        req_
                    );
                }

                // Invoke route handler and get custom b_net response class
                rr.route().handler()(req_, res.clear());

                // Send response, we finally handle the request here
                return b_net::create_response(res, req.version(), req.keep_alive(), req.method());
            }
            // Nested loop for routes in root route
            else if(rr.is_root(target))
            {
                for (auto& route : rr.routes())
                {
                    // Check is target match the route
                    // Continue the loop if there no match
                    if(!route.isTarget(target))
                        continue;

                    // Check request method
                    if(!route.methodAllowed(req.method()))
                    {
                        return b_net_errs::bad_request(
                            "Bad request. Method not allowed",
                            req_
                        );
                    }

                    // Check query parameters
                    if(!route.queriesExist(req_.queries()))
                    {
                        return b_net_errs::bad_request(
                            "Bad request. Not allowed query parameter(s)",
                            req_
                        );
                    }

                    // Invoke route handler and get custom b_net response class
                    route.handler()(req_, res.clear());

                    // Send response, we finally handle the request here
                    return b_net::create_response(res, req.version(), req.keep_alive(), req.method());
                }

                // If we not found request target aka route
                return b_net_errs::not_found(req_);
            }
            // Continue the loop if we didn't find a match
            else
                continue;
        }

        // If we not found request target aka route
        return b_net_errs::not_found(req_);
    }
}