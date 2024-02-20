#pragma once
#include "handle_route.hpp"

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request (
        b_net::Response& res,
        http::request<Body, http::basic_fields<Allocator>>&& beast_req,
        std::vector<b_net::RoutesContainer>& routes_containers
    )
    {
        using namespace b_net;

        /////////////////////////////////////////////////
        // Initializing of custom b_net request class //
        ////////////////////////////////////////////////

        // Declare variables here to prevent copying of them
        const std::string target = beast_req.target();

        // Check target - is it valid or not
        if (!utility_::isValid(target))
        {
            return b_net_errs::bad_request(
                "Bad request. Illegal request-target",
                beast_req.version(),
                beast_req.keep_alive(),
                utility_::convert_method(beast_req.method())
            );
        }

        // Redirrect, if there is no slash at the end of the target
        if(
            target.at(target.length() - 1) != '/'
            && target.find(".") == std::string::npos
        )
        {
            return b_net_errs::incorrect_ending_of_target(
                target,
                beast_req.version(),
                beast_req.keep_alive()
            );
        }

        std::list<ParsedField> queries;
        // Declare and get header fields
        std::list<ParsedField> header_fields;
        utility_::get_req_header_fields(beast_req.base(), header_fields);
        // Declare and parse cookies
        std::list<ParsedField> cookies;

        // Construct custom b_net request class for the handler
        Request req(
            // Request target, version and keep alive field
            target, beast_req.version(), beast_req.keep_alive(),
            // Request body and it's size
            beast_req.body(), beast_req.body().size(),
            // Get http header fields
            header_fields,
            // Parse and pass list of queries
            utility_::parse_queries(target, queries),
            // Parse and pass list of cookies
            utility_::parse_cookies(header_fields, cookies),
            // Convert beast method to b_net method
            utility_::convert_method(beast_req.method()),
            // Parse type of incoming file - how to decode octets
            utility_::mimeType_to_bodyType(utility_::parse_bodyType(beast_req.base()))
        );

        // Handle incoming request
        std::string nested_target{ "" };
        const std::vector<RoutesContainer>* current_iterable_container = &routes_containers;
        bool new_loop;

        while (true)
        {
            new_loop = false;

            for (auto& route : *current_iterable_container) {
                // Check if target is equal to current nesting route
                // And handle request if it's equal
                if (route.handler().isTarget(target, nested_target))
                {
                    return handle_route(req, res, route.handler());
                }

                // Nested loop for subroutes in routes container
                if (route.is_root(target))
                {
                    nested_target += route.target();
                    current_iterable_container = &route.sub_routes();
                    new_loop = true;
                    break;
                }
            }

            if (!new_loop)
                break;
        }

        // If we have not found request target aka route
        return b_net_errs::not_found(req);
    }
}