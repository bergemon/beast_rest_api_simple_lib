#pragma once
#include "../utility/utilityFunctions.hpp"
#include "errorCodes.hpp"

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request(http::request<Body, http::basic_fields<Allocator>>&& req) {
        // Make sure we can handle the method
        if (req.method() != http::verb::get
            && req.method() != http::verb::head
            && req.method() != http::verb::post)
            return ErrorCodes::bad_request(req.version(), req.keep_alive(), "Unknown HTTP-method");

        // Request path must be absolute and not contain ".." or "//".
        if (req.target().empty()
            || req.target()[0] != '/'
            || req.target().find("//") != beast::string_view::npos
            || req.target().find("..") != beast::string_view::npos)
            return ErrorCodes::bad_request(req.version(), req.keep_alive(), "Illegal request-target");

        // Build the path to the requested file
        std::string path = path_cat(".", req.target());
        if (req.target().back() == '/')
            path.append("index.html");

        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::scan, ec);

        // Handle the case where the file doesn't exist
        if(ec == beast::errc::no_such_file_or_directory)
            return ErrorCodes::not_found(req.version(), req.keep_alive(), req.target());

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