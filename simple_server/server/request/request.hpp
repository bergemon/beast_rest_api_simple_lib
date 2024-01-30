#pragma once
#include "../sharedData/sharedFunctions.hpp"

namespace b_net {
    class Request {
        std::string m_target;
        std::list<ParsedQuery>& m_queries;
        std::list<ParsedCookie>& m_cookies;
        b_net::Method m_method;

    public:
        Request(
            std::string target,
            std::list<ParsedQuery>& queries,
            std::list<ParsedCookie>& cookies,
            b_net::Method method
        )
            : m_target(target), m_queries(queries),
            m_method(method), m_cookies(cookies)
        { }

        // Request getters
        std::string target() const { return m_target; }
        const std::list<ParsedQuery>& queries() const { return m_queries; }
        const std::list<ParsedCookie>& cookies() const { return m_cookies; }
        b_net::Method method() const { return m_method; }
    };
}