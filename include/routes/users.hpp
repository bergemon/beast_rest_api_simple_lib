#pragma once
#include <string>
#include "../../simple_server/net_handler.hpp"

b_net::Response getUsers(std::string target, std::list<b_net::ParsedQuery>& queries, b_net::Method method);