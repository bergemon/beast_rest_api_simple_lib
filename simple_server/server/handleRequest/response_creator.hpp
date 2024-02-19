#pragma once
#include "../utility/utilityFunctions.hpp"

namespace b_net {
    http::message_generator create_response(
        b_net::Response& b_net_res,
        const uint32_t version,
        const bool keep_alive,
        const Method method
    )
    {
        using namespace b_net;

        // Forming boost beast response class
        if (b_net_res.get_size() > 0 && method != HEAD)
        {
            http::response<http::buffer_body> res{http::status::ok, version};
            res.set(http::field::server, "Rest API by bergemon");
            res.body().data = b_net_res.get_body();
            res.body().size = b_net_res.get_size();
            res.body().more = false;
            res.keep_alive(keep_alive);
            res.content_length(b_net_res.get_size());
            // Loop for all fields that we must set into http header
            for (const auto& elem : b_net_res.set_fields())
            {
                res.set(elem.name(), elem.value());
            }
            // Loop for all fields that we must insert into http header
            for (const auto& elem : b_net_res.insert_fields())
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
        for (const auto& elem : b_net_res.set_fields())
        {
            res.set(elem.name(), elem.value());
        }
        // Loop for all fields that we must insert into http header
        for (const auto& elem : b_net_res.insert_fields())
        {
            res.insert(elem.name(), elem.value());
        }
        res.prepare_payload();
        res.content_length(b_net_res.get_size());
        return res;
    }
}