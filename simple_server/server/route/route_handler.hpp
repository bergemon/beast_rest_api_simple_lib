#pragma once
#include "../response/response.hpp"

namespace b_net {
    class query final {
        const std::string m_query;
        const bool m_required;
        const query_type m_type;

    public:
        query(
            const std::string query,
            const bool required = false,
            const query_type type = query_type::STR_
        )
            : m_query(query), m_required(required), m_type(type)
        { }

        // Query getters
        std::string get_query () const { return m_query; }
        query_type get_type () const { return m_type; }
        bool isRequired () const { return m_required; }
    };

    class RouteHandler final {
        const slug_type m_slug_type;
        const std::string m_target;
        const std::vector<method> m_methods;
        const bool m_all_methods;
        const std::vector<query> m_queries;
        const std::function<void(Request&, Response&)> m_handler;

        const slug_type check_slug(const std::string target) const
        {
            return target.find("<int>") != std::string::npos
            ? INT_SLUG
            : target.find("<str>") != std::string::npos
            ? STR_SLUG
            : NO_SLUG;
        }

    public:
        RouteHandler(
            const std::vector<method> methods,
            const std::string target,
            const std::vector<query> queries,
            const std::function<void(Request&, Response&)> handler
        )
            : m_methods(methods), m_target(parse_target(target)),
            m_queries(queries), m_handler(handler),
            m_all_methods(hasAllMethod()), m_slug_type(check_slug(target))
        { }

        const bool hasAllMethod() const
        {
            for (const auto& elem : m_methods)
            {
                if(elem == method::ALL) { return true; }
            }
            return false;
        }

        const std::string parse_target(std::string target) const
        {
            if (m_slug_type != slug_type::NO_SLUG)
            {
                target = target.substr(0, target.rfind("/"));
            }

            return target;
        }

        const slug_type slug_type() const { return m_slug_type; }

        const bool isTarget(
            std::string target,
            const std::string nested_target,
            Request& req
        ) const
        {
            // target must be pure - without query parameters
            if (target.find("?") != std::string::npos)
            {
                target = target.substr(0, target.find("?"));
            }

            // trim the slash at the end of the target
            if (target.at(target.length() - 1) == '/')
            {
                target = target.substr(0, target.length() - 1);
            }

            // check if route has slug
            // cut the slug from the target
            if (slug_type() != slug_type::NO_SLUG)
            {
                const std::string slug = target.substr(target.rfind("/") + 1);
                req.m_slug = slug;

                target = target.substr(0, target.rfind("/"));
            }

            // compare request target and accumulated route target
            if (target == nested_target + m_target)
            {
                return true;
            }

            return false;
        }

        const bool queriesExist(
            const std::list<ParsedField> parsed_queries
        ) const
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
                    if (parsed_query.name() == query.get_query())
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
                    if (query.get_query() == parsed_query.name())
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

        const bool methodAllowed(const method req_method) const
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