#include "../../include/routes/users.hpp"

void getUsers(
    b_net::Request& req,
    b_net::Response& res
)
{
    int min, max;
    for (const auto& elem : req.queries()) {
        if (elem.name() == "min") {
            min = std::atoi(elem.value().c_str());
        }
        if (elem.name() == "max") {
            max = std::atoi(elem.value().c_str());
        }
    }

    std::stringstream ss;

    ss << "Test /users response!\n"
        << "Query parameters:\n"
        << "min = " << min << '\n'
        << "max = " << max;

    res.body(ss.str());
}