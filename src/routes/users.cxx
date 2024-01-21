#include "../../include/routes.hpp"

b_net::Response getUsers(std::string target, std::list<b_net::ParsedQuery>& queries, b_net::Method method) {

    b_net::Response res;

    int min, max;
    for (const auto& elem : queries) {
        if (elem.query() == "min") {
            min = std::atoi(elem.value().c_str());
        }
        if (elem.query() == "max") {
            max = std::atoi(elem.value().c_str());
        }
    }

    std::stringstream ss;

    ss << "Test /users response!\n"
        << "Query parameters:\n"
        << "min = " << min << '\n'
        << "max = " << max;

    res.body(ss.str());
    res.bodyType(TEXT);

    return res;
}