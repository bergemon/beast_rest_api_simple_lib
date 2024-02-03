#pragma once
#include "../utility/utilityFunctions.hpp"

http::message_generator createResponse(
    b_net::Response& r,
    const uint32_t version,
    const bool keep_alive,
    const http::verb method
)
{
    using namespace b_net;

    struct utility_class : public Response {
        // Parent class constructor
        utility_class(Response& response_class)
            : Response(response_class) { }
        // Getters for handler
        // Get pointer to body
        [[nodiscard]] unsigned char* getBody() { return m_body; }
        // Body length
        [[nodiscard]] size_t getSize() { return m_size; }
        // List of fields to set
        [[nodiscard]]
            std::list<Field>& set_fields() { return m_set_fields; }
        // List of field to insert
        [[nodiscard]]
            std::list<Field>& insert_fields() { return m_ins_fields; }
    };
    utility_class res_(r);

    // Forming boost beast response class
    if (res_.getSize() > 0 && method != http::verb::head)
    {
        http::response<http::buffer_body> res{http::status::ok, version};
        res.set(http::field::server, "Rest API by bergemon");
        res.body().data = res_.getBody();
        res.body().size = res_.getSize();
        res.body().more = false;
        res.keep_alive(keep_alive);
        res.content_length(res_.getSize());
        // Loop for all fields that we must set into http header
        for (const auto& elem : res_.set_fields())
        {
            res.set(elem.name(), elem.value());
        }
        // Loop for all fields that we must insert into http header
        for (const auto& elem : res_.insert_fields())
        {
            res.insert(elem.name(), elem.value());
        }
        return res;
    }

    // If response body is empty or request method was head
    http::response<http::empty_body> res{http::status::ok, version};
    res.set(http::field::server, "Rest API by bergemon");
    res.keep_alive(keep_alive);
    // Loop for all fields that we must set into http header
    for (const auto& elem : res_.set_fields())
    {
        res.set(elem.name(), elem.value());
    }
    // Loop for all fields that we must insert into http header
    for (const auto& elem : res_.insert_fields())
    {
        res.insert(elem.name(), elem.value());
    }
    res.prepare_payload();
    res.content_length(res_.getSize());
    return res;
}