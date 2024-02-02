#pragma once
#include "../sharedData/sharedFunctions.hpp"

namespace b_net {
    class Request {
        std::string m_target;
        char* m_body = nullptr;
        size_t m_body_size;
        std::list<ParsedQuery>& m_queries;
        std::list<ParsedCookie>& m_cookies;
        b_net::Method m_method;

    public:
        Request(
            std::string target,
            auto buffer,
            size_t size,
            std::list<ParsedQuery>& queries,
            std::list<ParsedCookie>& cookies,
            b_net::Method method
        )
            : m_target(target), m_queries(queries),
            m_method(method), m_cookies(cookies),
            m_body_size(size)
        {
            m_body = new char[m_body_size];
            for (const auto& elem : buffer.data())
            {
                for (size_t i = 0; i < m_body_size; ++i)
                {
                    m_body[i] = *asio::buffer_cast<char*>(elem + i);
                }
            }
        }

        // Request getters
        std::string target() const { return m_target; }
        const char* body() const { return reinterpret_cast<const char*>(m_body); }
        size_t body_size() const { return m_body_size; }
        const std::list<ParsedQuery>& queries() const { return m_queries; }
        const std::list<ParsedCookie>& cookies() const { return m_cookies; }
        b_net::Method method() const { return m_method; }
    };
}