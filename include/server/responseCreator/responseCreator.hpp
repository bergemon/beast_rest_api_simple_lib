#pragma once
#include "../route/route.hpp"

bergemon::Method convertMethod(http::verb method) {
    using namespace bergemon;

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

http::message_generator createResponse(
    bergemon::Response& r,
    uint32_t version,
    bool keep_alive
)
{
    bergemon::ChildResponse res_(r);

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