#pragma once
#include "../request/request.hpp"

namespace b_net {
    class Query {
        const std::string m_query;
        const bool m_required;
        const Type m_type;

    public:
        Query(const std::string query, const bool required = false, const Type type = Type::STR_)
            : m_query(query), m_required(required), m_type(type)
        { }

        std::string getQuery () const { return m_query; }
        Type getType () const { return m_type; }
        bool isRequired () const { return m_required; }
    };

    class Route {
        const bool m_all_methods;
        const std::vector<Method> m_methods;
        const std::string m_target;
        const std::vector<Query> m_queries;
        // Route handler
        const std::function
            // handler prototype
            <void(Request&, Response&)>
            m_handler;

    public:
        Route(const std::vector<Method> methods, const std::string target, const std::vector<Query> queries,
            const std::function
                // handler prototype
                <void(Request&, Response&)>
                handler
        )
            : m_methods(methods), m_target(target),
            m_queries(queries), m_handler(handler),
            m_all_methods(hasAllMethod())
        { }

        bool hasAllMethod() {
            for (const auto& elem : m_methods) {
                if(elem == Method::ALL) { return true; }
            }
            return false;
        }

        bool isValid (const std::string target = "0") {
            if (target.find("..") != std::string::npos
                || target.find("//") != std::string::npos
                || target.at(0) != '/'
                || target.length() < 2
                || target.find("?") > target.find("&")
                || target.find("&&") != std::string::npos
                || target.find("??") != std::string::npos)
            { return false; }

            return true;
        }

        bool isTarget (std::string target) {
            // target must be pure - without query parameters
            if (target.find("?") != std::string::npos)
                target = target.substr(0, target.find("?"));
            if (target.at(target.length() - 1) == '/')
                target = target.substr(0, target.length() - 1);

            if (target == m_target)
            { return true; }

            return false;
        }

        bool queriesExist (const std::list<ParsedQuery> parsed_queries) {
            bool found = false;

            // Required query must exist
            for (const auto& query : m_queries) {
                if (!query.isRequired()) { continue; }

                bool queryExist = false;

                for (const auto& parsed_query : parsed_queries) {
                    if (parsed_query.query() == query.getQuery()) {
                        queryExist = true;
                        break;
                    }
                }

                if (!queryExist) { return false; }
            }

            // Entered query must exist in the route            
            for (const auto& parsed_query : parsed_queries) {

                bool queryExist = false;

                for (const auto& query : m_queries) {
                    if (query.getQuery() == parsed_query.query()) {
                        queryExist = true;
                        break;
                    }
                }

                if (!queryExist) { return false; }
            }

            return true;
        }

        bool methodAllowed (const http::verb method) {
            if (m_all_methods) { return true; }

            for (const auto& elem : m_methods) {
                if (elem == Method::HEAD && method == http::verb::head)
                    return true;
                if (elem == Method::GET && method == http::verb::get)
                    return true;
                if (elem == Method::POST && method == http::verb::post)
                    return true;
                if (elem == Method::PUT && method == http::verb::put)
                    return true;
                if (elem == Method::DELETE_ && method == http::verb::delete_)
                    return true;
                if (elem == Method::PATCH && method == http::verb::patch)
                    return true;
            }

            return false;
        }

        const std::function
            // handler prototype
            <void(Request&, Response&)>
        handler() const
        {
            return m_handler;
        }
    };
}