#pragma once
#include "../response/response.hpp"

namespace b_net {
    class Query {
        const std::string m_query;
        const bool m_required;
        const Type m_type;

    public:
        Query(const std::string query, const bool required = false, const Type type = Type::STR_)
            : m_query(query), m_required(required), m_type(type)
        { }

        // Query getters
        std::string getQuery () const { return m_query; }
        Type getType () const { return m_type; }
        bool isRequired () const { return m_required; }
    };

    class RouteHandler {
        const std::string m_target;
        const std::vector<Method> m_methods;
        const bool m_all_methods;
        const std::vector<Query> m_queries;
        const std::function<void(Request&, Response&)> m_handler;

    public:
        RouteHandler(
            const std::vector<Method> methods,
            const std::string target,
            const std::vector<Query> queries,
            const std::function<void(Request&, Response&)> handler
        )
            : m_methods(methods), m_target(target),
            m_queries(queries), m_handler(handler),
            m_all_methods(hasAllMethod())
        { }

        const bool hasAllMethod() const
        {
            for (const auto& elem : m_methods)
            {
                if(elem == Method::ALL) { return true; }
            }
            return false;
        }

        const bool isTarget(std::string target, const std::string nested_target) const
        {
            // target must be pure - without query parameters
            if (target.find("?") != std::string::npos)
                target = target.substr(0, target.find("?"));
            if (target.at(target.length() - 1) == '/')
                target = target.substr(0, target.length() - 1);

            std::cout << "Target: " << target << std::endl;
            std::cout << "Nested target: " << nested_target << std::endl;
            std::cout << "Accumulated target: " << nested_target + m_target << std::endl;
            if (target == nested_target + m_target)
                return true;

            return false;
        }

        const bool queriesExist(const std::list<ParsedField> parsed_queries) const
        {
            bool found = false;

            // Required query must exist
            for (const auto& query : m_queries)
            {
                if (!query.isRequired()) 
                    continue;

                bool queryExist = false;

                for (const auto& parsed_query : parsed_queries)
                {
                    if (parsed_query.name() == query.getQuery())
                    {
                        queryExist = true;
                        break;
                    }
                }

                if (!queryExist)
                    return false;
            }

            // Entered query must exist in the route            
            for (const auto& parsed_query : parsed_queries)
            {
                bool queryExist = false;

                for (const auto& query : m_queries)
                {
                    if (query.getQuery() == parsed_query.name())
                    {
                        queryExist = true;
                        break;
                    }
                }

                if (!queryExist)
                    return false;
            }

            return true;
        }

        const bool methodAllowed(const Method req_method) const
        {
            if (m_all_methods)
                return true;

            for (const auto& elem : m_methods)
            {
                if (elem == req_method)
                    return true;
            }

            return false;
        }

        // Get route handler
        const std::function
            <void(Request&, Response&)>
            handler() const
        {
            return m_handler;
        }
    };
}