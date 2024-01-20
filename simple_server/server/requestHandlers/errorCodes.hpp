#pragma once

namespace ErrorCodes {
    // Returns a bad request response
    inline const http::message_generator bad_request(const uint32_t version, const bool keep_alive, const std::string why) {
        http::response<http::string_body> res{http::status::bad_request, version};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(keep_alive);
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    }

    // Returns a not found response
    inline const http::message_generator not_found(const uint32_t version, const bool keep_alive, const std::string target) {
        std::string pure_target = target.substr(1, target.find("?") - 1);

        http::response<http::string_body> res{http::status::not_found, version};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(keep_alive);
        res.body() = "The resource '" + pure_target + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    inline const http::message_generator server_error(const uint32_t version, const bool keep_alive, const std::string what) {
        http::response<http::string_body> res{http::status::internal_server_error, version};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(keep_alive);
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };
}