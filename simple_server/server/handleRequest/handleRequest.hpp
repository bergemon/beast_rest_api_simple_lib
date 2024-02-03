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
        // Handle incoming request
        for (auto& elem : routes) {
            using namespace b_net;

            // Check target - is it valid or not
            if (!elem.isValid(req.target()))
                return b_net_errs::bad_request(
                    "Bad request. Illegal request-target",
                    req.version(),
                    req.keep_alive(),
                    req.method()
                );

            // Check request target and skip current loop
            // If there is no such route target
            if (!elem.isTarget(req.target()))
                continue;

            // Check request method
            if (!elem.methodAllowed(req.method()))
                return b_net_errs::bad_request(
                    "Bad request. Method not allowed",
                    req.version(),
                    req.keep_alive(),
                    req.method()
                );

            // Declare variables here to prevent copying of them
            std::list<ParsedQuery> queries;
            std::list<ParsedCookie> cookies;
            std::string cookies_str;

            // Construct custom b_net request class for the handler
            b_net::Request req_(
                // Request target
                req.target(),
                // Request body
                req.body(),
                // Request body size
                req.body().size(),
                // Parse and pass list of queries
                utility_::parseQueries(req.target(), queries),
                // Parse and pass list of cookies
                utility_::parse_cookies(
                    utility_::get_cookie_field(req.base(), cookies_str),
                    cookies
                ),
                // Convert beast method to b_net method
                utility_::convertMethod(req.method()),
                // Parse type of incoming file - how to decode octets
                utility_::mimeType_to_bodyType(utility_::parse_bodyType(req.base()))
            );

            // Check query parameters
            if(!elem.queriesExist(req_.queries()))
                return b_net_errs::bad_request(
                    "Bad request. Not allowed query parameter(s)",
                    req.version(),
                    req.keep_alive(),
                    req.method()
                );

            // Invoke route handler and get custom b_net response class
            elem.handler()(req_, res.clear());

            return createResponse(res, req.version(), req.keep_alive(), req.method());
        }

        // If we not found request target aka route
        return b_net_errs::not_found(
            req.target(),
            req.version(),
            req.keep_alive(),
            req.method()
        );
    }
}