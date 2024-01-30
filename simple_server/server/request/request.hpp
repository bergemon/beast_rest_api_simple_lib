#pragma once
#include "../response/response.hpp"

namespace b_net {

    class Request {
        std::string m_target;
        std::list<ParsedQuery> m_queries;
        b_net::Method m_method;

    public:
        Request(std::string target, std::list<ParsedQuery> queries, b_net::Method method)
            : m_target(target), m_queries(queries), m_method(method)
        { }

        std::string target() const { return m_target; }

        std::list<ParsedQuery>& queries() { return m_queries; }

        b_net::Method method() const { return m_method; }
    };
}