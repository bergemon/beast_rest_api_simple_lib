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

            if (!elem.isValid(req.target()))
                return bad_request("Bad request. Illegal request-target");

            if (!elem.isTarget(req.target()))
                continue;

            if (!elem.methodAllowed(req.method()))
                return bad_request("Bad request. Method not allowed");

            std::list<ParsedQuery> queries_list = elem.parseQueries(req.target());

            if(!elem.queriesExist(queries_list))
                return bad_request("Bad request. Not allowed query parameter(s)");

            elem.handler()(
                res,
                req.target(),
                queries_list,
                convertMethod(req.method())
            );

            return createResponse(res, req.version(), req.keep_alive());
        }

        // If there is no such requested target
        std::string res_string = req.target();
        if (res_string.find("?") != std::string::npos)
            res_string = res_string.substr(0, res_string.find("?"));

        return not_found(res_string);

        // Build the path to the requested file
        // std::string path = path_cat(".", req.target());
        // if (req.target().back() == '/')
        //     path.append("index.html");

        // Attempt to open the file
        // beast::error_code ec;
        // http::file_body::value_type body;
        // body.open(path.c_str(), beast::file_mode::scan, ec);

        // Handle an unknown error
        // if(ec)
        //     return ErrorCodes::server_error(req.version(), req.keep_alive(), ec.message());

        // Cache the size since we need it after the move
        // auto const size = body.size();
    }
}