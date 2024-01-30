#pragma once
#include "../route/route.hpp"

inline http::message_generator createResponse(
    b_net::Response& r,
    uint32_t version,
    bool keep_alive
)
{
    struct utility_class : public b_net::Response {
        // Parent class constructor
        utility_class(b_net::Response& response_class)
            : Response(response_class) { }
        // Getters for handler
        b_net::BodyType getType() { return m_type; }
        [[nodiscard]] unsigned char* getBody() { return m_body; }
        // Body length
        [[nodiscard]] size_t getSize() { return m_size; }
        // List of fields to set
        [[nodiscard]] std::list<b_net::Field>& getSetFields()
        { return m_setFields; }
        // List of field to insert
        [[nodiscard]] std::list<b_net::Field>& getInsertFields()
        { return m_insertFields; }
    };
    utility_class res_(r);

    // Response type - text
    if (res_.getSize() && res_.getType() == b_net::BodyType::TEXT)
    {
        http::response<http::string_body> res{http::status::ok, version};
        res.set(http::field::server, "Rest API by bergemon");
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(keep_alive);
        res.body() = reinterpret_cast<char*>(res_.getBody());
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

    // Response type - binary
    if (res_.getSize() && res_.getType() == b_net::BodyType::BINARY)
    {
        http::response<http::buffer_body> res{http::status::ok, version};
        res.set(http::field::server, "Rest API by bergemon");
        res.body().data = res_.getBody();
        res.body().size = res_.getSize();
        res.body().more = false;
        res.keep_alive(keep_alive);
        res.content_length(res_.getSize());
        // Loop for all fields that we must set into http header
        for (const auto& elem : res_.getSetFields()) {
            res.set(elem.getName(), elem.getValue());
        }
        // Loop for all fields that we must insert into http header
        for (const auto& elem : res_.getInsertFields()) {
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