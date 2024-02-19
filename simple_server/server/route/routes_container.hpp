#pragma once
#include "route_handler.hpp"

namespace b_net {
    class RoutesContainer;
}

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request (
        b_net::Response& res,
        http::request<Body, http::basic_fields<Allocator>>&& req,
        std::vector<b_net::RoutesContainer>& root_routes
    );
}

namespace b_net {
    class RoutesContainer final {
        // Routes container always must keep it's part of target
        const std::string m_target;
        // Current route nesting level
        const uint32_t m_route_nest;
        // Root route handler
        std::optional<RouteHandler> m_route_handler;
        // Routes of root route aka subroutes
        std::vector<RoutesContainer> m_routes;

        // Private method only for request handler
        // Check if requested target contains root route
        // So we check is this target is part of this root route or not
        const bool is_root(std::string parsed_target) const
        {
            uint32_t nesting = 0;

            // Cutting first half of extra words
            while(
                parsed_target.find("/") != std::string::npos
                && nesting < m_route_nest
            )
            {
                uint32_t slash_pos = 1;

                for (; slash_pos < parsed_target.length(); ++slash_pos)
                {
                    if (parsed_target[slash_pos] == '/')
                        break;
                }

                parsed_target = parsed_target.substr(slash_pos);
                ++nesting;
            }

            // Cutting second half of extra words
            if(parsed_target.find("/") != std::string::npos)
            {
                uint32_t pos = 0;
                for (; pos < parsed_target.length(); ++pos)
                {
                    if(parsed_target[pos] == '/' && pos > 0)
                        break;
                }
                parsed_target = parsed_target.substr(0, pos);
            }

            std::cout << "\n\nParsed target: " << parsed_target << std::endl;
            std::cout << "Route nest: " << m_route_nest << std::endl;
            std::cout << "Root target: " << m_target << "\n\n" << std::endl;
            return parsed_target == m_target;
        }

        template <class Body, class Allocator>
        friend http::message_generator HandleRequest::handle_request (
                b_net::Response& res,
                http::request<Body, http::basic_fields<Allocator>>&& req,
                std::vector<b_net::RoutesContainer>& root_routes
            );

    public:
        // Empty root route
        RoutesContainer(const std::string target, const uint32_t nesting)
            : m_target(target), m_route_nest(nesting)
        { }

        // Root route with queries
        RoutesContainer(
            const std::vector<Method> methods,
            const std::string target,
            const std::vector<Query> queries,
            const std::function<void(Request&, Response&)> handler,
            const uint32_t nesting
        )
            : m_target(target), m_route_nest(nesting)
        { m_route_handler.emplace(RouteHandler(methods, target, queries, handler)); }

        // Root route without queries
        RoutesContainer(
            const std::vector<Method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler,
            const uint32_t nesting
        )
            : m_target(target), m_route_nest(nesting)
        { m_route_handler.emplace(RouteHandler(methods, target, {}, handler)); }

        ////////////////////////////////////////////
        // METHODS
        ////////////////////////////////////////////

        // Set handlers for the root route
        // Set root route handler with queries
        void SET_HANDLER(
            const std::vector<Method> methods,
            const std::vector<Query> queries,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_route_handler.emplace(RouteHandler(methods, m_target, queries, handler));
        }
        // Set root route handler without queries
        void SET_HANDLER(
            const std::vector<Method> methods,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_route_handler.emplace(RouteHandler(methods, m_target, {}, handler));
        }

        // New routes of these root route
        // Put new route to handle by the server (with queries)
        [[nodiscard]] RoutesContainer& SUB_ROUTE(
            const std::vector<Method> methods,
            const std::string target,
            const std::vector<Query> queries,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_routes.emplace_back(methods, target, queries, handler, m_route_nest + 1);
            return m_routes.back();
        }
        // Put new route to handle by the server (without queries)
        [[nodiscard]] RoutesContainer& SUB_ROUTE(
            const std::vector<Method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_routes.emplace_back(methods, target, std::vector<Query>(), handler, m_route_nest + 1);
            return m_routes.back();
        }

        // Get root target
        const std::string& target() const { return m_target; }

        // Root route handler getter
        const RouteHandler& handler() const { return *m_route_handler; }

        // Get array of routes
        const std::vector<RoutesContainer>& sub_routes() const { return m_routes; }
    };
}