#pragma once
#include "responseCreator.hpp"

namespace b_net_errs {
    // Error codes returning
    // Bad request
    http::message_generator
        bad_request
        (const std::string why, const int version, 
        const bool keep_alive, const http::verb method)
    {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(keep_alive);
        if (method != http::verb::head)
        {
            res.body() = why;
        }
        res.prepare_payload();
        return res;
    };
    // Not found
    http::message_generator
        not_found
        (std::string target, const int version,
        const bool keep_alive, const http::verb method)
    {
        if (target.find("?") != std::string::npos)
            target = target.substr(1, target.find("?") - 1);
        else
            target = target.substr(1);

        http::response<http::string_body> res{http::status::not_found, version};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(keep_alive);
        if (method != http::verb::head)
        {
            res.body() = "The resource '" + target + "' was not found.";
        }
        res.prepare_payload();
        return res;
    };
    // Server error
    http::message_generator
        server_error
        (const std::string what, const int version,
        const bool keep_alive, const http::verb method)
    {
        http::response<http::string_body> res{http::status::internal_server_error, version};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(keep_alive);
        if (method != http::verb::head)
        {
            res.body() = "An error occurred: '" + what + "'";
        }
        res.prepare_payload();
        return res;
    };
}