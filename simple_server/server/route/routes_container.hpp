#pragma once
#include "route_handler.hpp"

namespace b_net {
    class Server;
    class RoutesContainer;
}

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request (
        b_net::Response&,
        http::request<Body, http::basic_fields<Allocator>>&&,
        const std::list<b_net::RoutesContainer>*
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
        std::list<RoutesContainer> m_routes;

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
            
            return parsed_target == m_target;
        }

        // Returns true if this route has a slug
        const bool has_slug() const {
            return m_target.find("/<int>") != std::string::npos
                || m_target.find("/<str>") != std::string::npos;
        }

        void valid_slug() const
        {
            // Return false if route hasn't handler but has a slug
            if (has_slug() && !m_route_handler.has_value())
            {
                throw std::runtime_error("Route without handler can't have a slug");
            }

            // Or if route has a slug and nested routes
            if (has_slug() && m_routes.size())
            {
                throw std::runtime_error("Route with slug can't have subroutes");
            }
        }

        // Get root target
        const std::string& target() const { return m_target; }

        // Root route handler getter
        const RouteHandler& handler() const { return *m_route_handler; }

        // Get array of routes
        const std::list<RoutesContainer>& sub_routes() const { return m_routes; }

        friend class Server;

        template <class Body, class Allocator>
        friend http::message_generator HandleRequest::handle_request (
            b_net::Response&,
            http::request<Body, http::basic_fields<Allocator>>&&,
            const std::list<b_net::RoutesContainer>*
        );

    public:
        // Empty root route
        RoutesContainer(const std::string target, const uint32_t nesting)
            : m_target(target), m_route_nest(nesting)
        { }

        // Root route with queries
        RoutesContainer(
            const std::vector<method> methods,
            const std::string target,
            const std::vector<query> queries,
            const std::function<void(Request&, Response&)> handler,
            const uint32_t nesting
        )
            : m_target(target), m_route_nest(nesting)
        { m_route_handler.emplace(methods, target, queries, handler); }

        // Root route without queries
        RoutesContainer(
            const std::vector<method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler,
            const uint32_t nesting
        )
            : m_target(target), m_route_nest(nesting)
        { m_route_handler.emplace(methods, target, std::vector<query>(), handler); }

        ////////////////////////////////////////////
        // METHODS
        ////////////////////////////////////////////

        // Set handlers for the root route
        // Set root route handler with queries
        void SET_HANDLER(
            const std::vector<method> methods,
            const std::vector<query> queries,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_route_handler.emplace(methods, m_target, queries, handler);
        }
        // Set root route handler without queries
        void SET_HANDLER(
            const std::vector<method> methods,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_route_handler.emplace(methods, m_target, std::vector<query>(), handler);
        }

        // New routes of these root route
        // Empty new route - root route
        [[nodiscard]] RoutesContainer& SUB_ROUTE(
            const std::string target
        )
        {
            m_routes.emplace_back(target, m_route_nest + 1);
            return m_routes.back();
        }
        // Put new route to handle by the server (with queries)
        [[nodiscard]] RoutesContainer& SUB_ROUTE(
            const std::vector<method> methods,
            const std::string target,
            const std::vector<query> queries,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_routes.emplace_back(methods, target, queries, handler, m_route_nest + 1);
            return m_routes.back();
        }
        // Put new route to handle by the server (without queries)
        [[nodiscard]] RoutesContainer& SUB_ROUTE(
            const std::vector<method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_routes.emplace_back(methods, target, std::vector<query>(), handler, m_route_nest + 1);
            return m_routes.back();
        }

        // Subroutes for static files
        void static_file(const std::string path)
        {
            using namespace b_net;

            const std::string target{ path.substr((path.rfind("/") == std::string::npos) ? 0 : (path.rfind("/") + 1)) };

            m_routes.emplace_back(
                std::vector<method>({ GET }),
                std::string('/' + target),
                std::vector<query>(),
                std::function<void(Request&, Response&)>(
                    [path, target](Request& req, Response& res) -> void
                    {
                        error_code ec = res.file_body(path);

                        if(ec.error())
                            throw std::runtime_error("File " + target + " not found");
                    }
                ),
                m_route_nest + 1
            );
        }
    };
}