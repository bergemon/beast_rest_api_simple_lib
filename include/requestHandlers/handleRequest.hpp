#pragma once
#include "../utility/utilityFunctions.hpp"
#include "../server/route.hpp"
#include "errorCodes.hpp"

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request(
        http::request<Body, http::basic_fields<Allocator>>&& req,
        std::vector<bergemon::Route>& routes)
    {
        for (const auto& elem : routes) {
            if (!elem.isValid(req.target()))
                return ErrorCodes::bad_request(req.version(), req.keep_alive(), "Illegal request-target");

            if (!elem.isTarget(req.target()))
                return ErrorCodes::not_found(req.version(), req.keep_alive(), req.target());

            if (!elem.methodAllowed(req.method()))
                return ErrorCodes::bad_request(req.version(), req.keep_alive(), "Unknown HTTP-method");
        }

        // Build the path to the requested file
        std::string path = path_cat(".", req.target());
        if (req.target().back() == '/')
            path.append("index.html");

        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::scan, ec);

        // Handle an unknown error
        if(ec)
            return ErrorCodes::server_error(req.version(), req.keep_alive(), ec.message());

        // Cache the size since we need it after the move
        auto const size = body.size();

        // Respond to HEAD request
        if(req.method() == http::verb::head) {
            http::response<http::empty_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());
            return res;
        }

        // Respond to GET request
        http::response<http::file_body> res{std::piecewise_construct,
            std::make_tuple(std::move(body)),
            std::make_tuple(http::status::ok, req.version())};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return res;
    }
}