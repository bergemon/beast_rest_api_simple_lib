#pragma once
#include "../sharedData/sharedFunctions.hpp"

namespace b_net {
    class RouteHandler;

    class Request final {
        std::string m_target;
        uint32_t m_version;
        bool m_keep_alive;
        char* m_body = nullptr;
        size_t m_body_size;
        std::list<ParsedField>& m_fields;
        std::list<ParsedField>& m_queries;
        std::list<ParsedField>& m_cookies;
        Method m_method;
        BodyType m_type;
        std::string m_slug;

        friend class RouteHandler;

    public:
        Request(
            std::string target,
            uint32_t version,
            bool keep_alive,
            auto buffer,
            size_t size,
            std::list<ParsedField>& fields,
            std::list<ParsedField>& queries,
            std::list<ParsedField>& cookies,
            Method method,
            BodyType type
        )
            : m_target(target), m_version(version),
            m_keep_alive(keep_alive), m_fields(fields),
            m_queries(queries), m_method(method),
            m_cookies(cookies), m_body_size(size),
            m_type(type)
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

        // Request target, this method returns copy of the string
        std::string target() const { return m_target; }
        
        // Version of http request, presented by 32 bit integer variable
        const uint32_t version() const { return m_version; }

        // Keep-Alive header field, presented by boolean variable
        const bool keep_alive() const { return m_keep_alive; }

        // Pointer to request body
        const char* body() const { return reinterpret_cast<const char*>(m_body); }

        // Size of the body in bytes
        size_t body_size() const { return m_body_size; }

        // List of parsed header fields names and it's values
        const std::list<ParsedField>& header_fields() const { return m_fields; }

        // List of parsed query parameters and it's values
        const std::list<ParsedField>& queries() const { return m_queries; }

        // List of parsed cookies and it's values
        const std::list<ParsedField>& cookies() const { return m_cookies; }

        // Request method
        Method method() const { return m_method; }

        // Extension of the request body (.jpg for example or .html)
        BodyType type() const { return m_type; }

        // Get the slug
        std::string slug() const { return m_slug; }

        // Get mime type aka file extension
        const std::string mime_type()
        {
            return utility_::bodyType_to_mimeType(m_type);
        }
    };
}