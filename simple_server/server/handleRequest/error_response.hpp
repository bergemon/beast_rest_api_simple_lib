#pragma once
#include "response_creator.hpp"

namespace b_net_errs {
    // Error codes returning
    // Return bad request - 400
    http::message_generator
        bad_request
        (const std::string why, uint32_t version, bool keep_alive, b_net::Method method)
    {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(keep_alive);
        if (method != HEAD)
        {
            res.body() = why;
        }
        res.prepare_payload();
        return res;
    };

    http::message_generator
        static_file_not_found
        (const b_net::Request& req, const std::string message)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.version());
        res.body() = message;
        res.prepare_payload();
        return res;
    }

    // Return not found error - 404
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

    // Return server error - 500
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

    // Return permanent redirrect - 301
    http::message_generator
        incorrect_ending_of_target
        (const std::string target, uint32_t version, bool keep_alive)
    {
        http::response<http::string_body> res{http::status::moved_permanently, version};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::location, target + '/');
        res.keep_alive(keep_alive);
        res.prepare_payload();
        return res;
    };
}