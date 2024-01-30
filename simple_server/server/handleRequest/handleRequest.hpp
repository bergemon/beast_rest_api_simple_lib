#pragma once
#include "../responseCreator/responseCreator.hpp"

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request (
        b_net::Response& res,
        http::request<Body, http::basic_fields<Allocator>>&& req,
        std::vector<b_net::Route>& routes
    )
    {
        // Error codes returning
        // Bad request
        const auto bad_request = [&req](std::string why) {
            http::response<http::string_body> res{http::status::bad_request, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = std::string(why);
            res.prepare_payload();
            return res;
        };
        // Not found
        const auto not_found = [&req](std::string target) {
            std::string pure_target = target.substr(1, target.find("?") - 1);

            http::response<http::string_body> res{http::status::not_found, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "The resource '" + pure_target + "' was not found.";
            res.prepare_payload();
            return res;
        };
        // Server error
        const auto server_error = [&req](std::string what) {
            http::response<http::string_body> res{http::status::internal_server_error, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "An error occurred: '" + std::string(what) + "'";
            res.prepare_payload();
            return res;
        };

        // Handle incoming request
        for (auto& elem : routes) {
            using namespace b_net;

            // Check target - is it valid or not
            if (!elem.isValid(req.target()))
                return bad_request("Bad request. Illegal request-target");

            // Check request target and skip current loop
            // If there is no such route target
            if (!elem.isTarget(req.target()))
                continue;

            // Check request method
            if (!elem.methodAllowed(req.method()))
                return bad_request("Bad request. Method not allowed");

            // Declare variables here to prevent copying of them
            std::list<ParsedQuery> queries;
            std::list<ParsedCookie> cookies;
            std::string cookies_str;

            // Construct custom b_net request class for the handler
            b_net::Request req_(
                // Request target
                req.target(),
                // Parse and pass list of queries
                utility_::parseQueries(req.target(), queries),
                // Parse and pass list of cookies
                utility_::parse_cookies(
                    utility_::get_cookie_field(req.base(), cookies_str),
                    cookies
                ),
                // Convert beast method to b_net method
                utility_::convertMethod(req.method())
            );

            // Check query parameters
            if(!elem.queriesExist(req_.queries()))
                return bad_request("Bad request. Not allowed query parameter(s)");

            // Invoke route handler and get custom b_net response class
            elem.handler()(req_, res.clear());

            return createResponse(res, req.version(), req.keep_alive());
        }

        // If there is no such requested target
        std::string res_string = req.target();
        if (res_string.find("?") != std::string::npos)
            res_string = res_string.substr(0, res_string.find("?"));

        return not_found(res_string);
    }
}