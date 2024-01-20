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
    b_net::ChildResponse res_(r);

    http::response<http::string_body> res{http::status::ok, version};
    res.set(http::field::server, "Rest API by bergemon");
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(keep_alive);
    res.body() = res_.getBody();
    res.content_length(res_.getSize());
    // Loop for all fields that we must set into http header
    for (const auto& elem : res_.getSetFields()) {
        res.set(elem.getName(), elem.getValue());
    }
    // Loop for all fields that we must insert into http header
    for (const auto& elem : res_.getInsertFields()) {
        res.insert(elem.getName(), elem.getValue());
    }
    res.prepare_payload();
    return res;
}