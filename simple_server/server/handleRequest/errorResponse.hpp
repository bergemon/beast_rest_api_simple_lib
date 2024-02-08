#pragma once
#include "responseCreator.hpp"

namespace b_net_errs {
    // Error codes returning
    // Bad request
    http::message_generator
        bad_request
        (const std::string why, const b_net::Request& req)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        if (req.method() != HEAD)
        {
            res.body() = why;
        }
        res.prepare_payload();
        return res;
    };
    // Not found
    http::message_generator
        not_found
        (const b_net::Request& req)
    {
        std::string target = req.target();

        if (target.find("?") != std::string::npos)
        {
            req.target() = target.substr(1, target.find("?") - 1);
        }
        else
        {
            target = target.substr(1);
        }

        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.version());
        if (req.method() != HEAD)
        {
            res.body() = "The resource '" + target + "' was not found.";
        }
        res.prepare_payload();
        return res;
    };
    // Server error
    http::message_generator
        server_error
        (const std::string what, const b_net::Request& req)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        if (req.method() != HEAD)
        {
            res.body() = "An error occurred: '" + what + "'";
        }
        res.prepare_payload();
        return res;
    };
}