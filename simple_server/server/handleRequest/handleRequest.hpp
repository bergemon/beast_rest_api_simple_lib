#pragma once
#include "errorResponse.hpp"

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request (
        b_net::Response& res,
        http::request<Body, http::basic_fields<Allocator>>&& req,
        std::vector<b_net::Route>& routes
    )
    {
        /////////////////////////////////////////////////
        // Initializing of custom b_net request class //
        ////////////////////////////////////////////////

        // Declare variables here to prevent copying of them
        std::list<ParsedField> queries;
        // Declare and get header fields
        std::list<ParsedField> header_fields;
        utility_::get_req_header_fields(req.base(), header_fields);
        // Declare and parse cookies
        std::list<ParsedField> cookies;
        std::string cookie_field;

        // Construct custom b_net request class for the handler
        b_net::Request req_(
            // Request target, version and keep alive field
            req.target(), req.version(), req.keep_alive(),
            // Request body and it's size
            req.body(), req.body().size(),
            // Get http header fields
            header_fields,
            // Parse and pass list of queries
            utility_::parse_queries(req.target(), queries),
            // Parse and pass list of cookies
            utility_::parse_cookies(header_fields, cookies),
            // Convert beast method to b_net method
            utility_::convert_method(req.method()),
            // Parse type of incoming file - how to decode octets
            utility_::mimeType_to_bodyType(utility_::parse_bodyType(req.base()))
        );

        // Handle incoming request
        for (auto& elem : routes) {
            using namespace b_net;

            // Check target - is it valid or not
            if (!elem.isValid(req.target()))
                return b_net_errs::bad_request(
                    "Bad request. Illegal request-target",
                    req_
                );

            // Check request target and skip current loop
            // If there is no such route target
            if (!elem.isTarget(req.target()))
                continue;

            // Check request method
            if (!elem.methodAllowed(req.method()))
                return b_net_errs::bad_request(
                    "Bad request. Method not allowed",
                    req_
                );

            // Check query parameters
            if(!elem.queriesExist(req_.queries()))
                return b_net_errs::bad_request(
                    "Bad request. Not allowed query parameter(s)",
                    req_
                );

            // Invoke route handler and get custom b_net response class
            elem.handler()(req_, res.clear());

            return b_net::create_response(res, req.version(), req.keep_alive(), req.method());
        }

        // If we not found request target aka route
        return b_net_errs::not_found(req_);
    }
}