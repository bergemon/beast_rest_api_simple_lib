#pragma once
#include "../response/response.hpp"

namespace b_net {

    enum Type { STR_, INT_, BOOL_ };

    enum Method { HEAD, GET, POST, PUT, DELETE_, PATCH };

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

    class ParsedQuery {
        const std::string m_query;
        const std::string m_value;
        
    public:
        ParsedQuery(std::string query, std::string value)
            : m_query(query), m_value(value) {}
        std::string query() const { return m_query; }
        std::string value() const { return m_value; }
    };

    class Route {
        const std::vector<Method> m_methods;
        const std::string m_target;
        const std::vector<Query> m_queries;
        // Route handler
        const std::function
            // handler prototype
            <Response(std::string, std::list<ParsedQuery>&, Method)>
            m_handler;

    public:
        Route(const std::vector<Method> methods, const std::string target, const std::vector<Query> queries,
            const std::function
                // handler prototype
                <Response(std::string, std::list<ParsedQuery>&, Method)>
                handler
        )
            : m_methods(methods), m_target(target), m_queries(queries), m_handler(handler)
        { }

        bool isValid (const std::string target = "0") {
            if (target.find("..") != std::string::npos
                || target.find("//") != std::string::npos
                || target.at(0) != '/'
                || target.length() < 2)
            { return false; }

            return true;
        }

        bool isTarget (const std::string target) {
            if (m_queries.size() == 0 && target == m_target
                || m_queries.size() > 0
                && target.substr(0, target.find("?")) == m_target
                || target.substr(0, target.length() - 1) == m_target)
            { return true; }

            return false;
        }

        std::list<ParsedQuery> parseQueries (const std::string target) {
            std::list<ParsedQuery> queries;
            bool last = false;
            std::string tempStr{ target.substr(target.find("?") + 1,
                target.length() - (target.find("?") + 1)) };

            while(tempStr.find("&") != std::string::npos || last == false) {
                if (tempStr.find("&") == std::string::npos) { last = true; }

                if (last == false) {
                    queries.push_back({
                        tempStr.substr(0, tempStr.find("=")),
                        tempStr.substr(tempStr.find("=") + 1, tempStr.find("&") - (tempStr.find("=") + 1))
                    });
                    tempStr =
                        tempStr.substr(tempStr.find("&") + 1, tempStr.length() - (tempStr.find("&") + 1));
                }
                else {
                    queries.push_back({
                        tempStr.substr(0, tempStr.find("=")),
                        tempStr.substr(tempStr.find("=") + 1, tempStr.length() - (tempStr.find("=") + 1))
                    });
                }
            }

            return queries;
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
            <Response(std::string, std::list<ParsedQuery>&, Method)>
        handler() const
        {
            return m_handler;
        }
    };
}

// Type of query
using b_net::Type::BOOL_;
using b_net::Type::INT_;
using b_net::Type::STR_;

// Methods
using b_net::Method::DELETE_;
using b_net::Method::GET;
using b_net::Method::HEAD;
using b_net::Method::PATCH;
using b_net::Method::POST;
using b_net::Method::PUT;

// Parameters for handler prototype
using b_net::ParsedQuery;
using b_net::Method;