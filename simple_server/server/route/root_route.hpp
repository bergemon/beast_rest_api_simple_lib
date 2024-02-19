#pragma once
#include "route.hpp"

namespace b_net {
    class RootRoute;
}

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request (
        b_net::Response& res,
        http::request<Body, http::basic_fields<Allocator>>&& req,
        std::vector<b_net::RootRoute>& root_routes
    );
}

namespace b_net {
    class RootRoute final {
        const std::string m_root_target;
        // Root route handler
        std::optional<Route> m_root_route;
        // Routes of root route aka subroutes
        std::vector<Route> m_routes;

        // Private method only for request handler
        // Check if requested target contains root route
        // So we check is this target is part of this root route or not
        const bool is_root(const std::string target) const
        {
            uint32_t sec_slash_pos{ 1 };
            for (; sec_slash_pos < target.length(); ++sec_slash_pos)
            {
                if (target[sec_slash_pos] == '/')
                    break;
            }
            std::string temp_str = target.substr(0, sec_slash_pos);

            return temp_str == m_root_target;
        }

        template <class Body, class Allocator>
        friend http::message_generator HandleRequest::handle_request (
                b_net::Response& res,
                http::request<Body, http::basic_fields<Allocator>>&& req,
                std::vector<b_net::RootRoute>& root_routes
            );

    public:
        // Empty root route
        RootRoute(const std::string target)
            : m_root_target(target)
        { }

        // Root route with queries
        RootRoute(
            const std::vector<Method> methods,
            const std::string target,
            const std::vector<Query> queries,
            const std::function<void(Request&, Response&)> handler
        )
            : m_root_target(target)
        { m_root_route.emplace(methods, target, queries, handler); }

        // Root route without queries
        RootRoute(
            const std::vector<Method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler
        )
            : m_root_target(target)
        { m_root_route.emplace(methods, target, std::vector<Query>(), handler); }

        ////////////////////////////////////////////
        // METHODS
        ////////////////////////////////////////////

        // Set handlers for the root route
        // Set root route handler with queries
        void SET_HANDLER(
            const std::vector<Method> methods,
            const std::string target,
            const std::vector<Query> queries,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_root_route.emplace(methods, target, queries, handler);
        }
        // Set root route handler without queries
        void SET_HANDLER(
            const std::vector<Method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_root_route.emplace(methods, target, std::vector<Query>(), handler);
        }

        // New routes of these root route
        // Put new route to handle by the server (with queries)
        const Route& ROUTE(
            const std::vector<Method> methods,
            const std::string target,
            const std::vector<Query> queries,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_routes.emplace_back(methods, target, queries, handler);
            return m_routes.back();
        }
        // Put new route to handle by the server (without queries)
        const Route& ROUTE(
            const std::vector<Method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_routes.emplace_back(methods, target, std::vector<Query>(), handler);
            return m_routes.back();
        }

        // Get root target
        const std::string& target() const { return m_root_target; }

        // Root route handler getter
        const Route& route() const { return *m_root_route; }

        // Get array of routes
        const std::vector<Route>& routes() const { return m_routes; }
    };
}