#pragma once
#include "../route/route.hpp"

inline b_net::Method convertMethod(http::verb method) {
    using namespace b_net;

    switch(method) {
        case http::verb::head:
            return Method::HEAD;
        case http::verb::get:
            return Method::GET;
        case http::verb::post:
            return Method::POST;
        case http::verb::put:
            return Method::PUT;
        case http::verb::delete_:
            return Method::DELETE_;
        case http::verb::patch:
            return Method::PATCH;
        default:
            return Method::HEAD;
    }
}

inline http::message_generator createResponse(
    b_net::Response& r,
    uint32_t version,
    bool keep_alive
)
{
    b_net::utility_class res_;

    // Response type - text
    if (res_.getSize(r) && res_.getType(r) == b_net::BodyType::TEXT)
    {
        http::response<http::string_body> res{http::status::ok, version};
        res.set(http::field::server, "Rest API by bergemon");
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(keep_alive);
        res.body() = reinterpret_cast<char*>(res_.getBody(r));
        res.content_length(res_.getSize(r));
        // Loop for all fields that we must set into http header
        for (const auto& elem : res_.getSetFields(r)) {
            res.set(elem.getName(), elem.getValue());
        }
        // Loop for all fields that we must insert into http header
        for (const auto& elem : res_.getInsertFields(r)) {
            res.insert(elem.getName(), elem.getValue());
        }
        res.prepare_payload();
        return res;
    }

    // Response type - binary
    if (res_.getSize(r) && res_.getType(r) == b_net::BodyType::BINARY)
    {
        http::response<http::buffer_body> res{http::status::ok, version};
        res.set(http::field::server, "Rest API by bergemon");
        // res.set(http::field::content_type, "text/html; charset=windows-1251");
        res.set(http::field::content_type, "image/jpeg");
        // res.set(http::field::content_type, "application/octet-stream");
        res.body().data = res_.getBody(r);
        res.body().size = res_.getSize(r);
        res.body().more = false;
        res.keep_alive(keep_alive);
        res.content_length(res_.getSize(r));
        // Loop for all fields that we must set into http header
        for (const auto& elem : res_.getSetFields(r)) {
            res.set(elem.getName(), elem.getValue());
        }
        // Loop for all fields that we must insert into http header
        for (const auto& elem : res_.getInsertFields(r)) {
            res.insert(elem.getName(), elem.getValue());
        }
        return res;
    }

    // If response body is empty
    http::response<http::empty_body> res{http::status::ok, version};
    res.set(http::field::server, "Rest API by bergemon");
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(keep_alive);
    // Loop for all fields that we must set into http header
    for (const auto& elem : res_.getSetFields(r)) {
        res.set(elem.getName(), elem.getValue());
    }
    // Loop for all fields that we must insert into http header
    for (const auto& elem : res_.getInsertFields(r)) {
        res.insert(elem.getName(), elem.getValue());
    }
    res.prepare_payload();
    return res;
}