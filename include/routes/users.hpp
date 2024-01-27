#pragma once
#include <string>
#include "../../simple_server/net_handler.hpp"

void getUsers(
    b_net::Response& res,
    std::string target,
    std::list<b_net::ParsedQuery>& queries,
    b_net::Method method
);