#pragma once
#include "server/HTTP/listener.hpp"

namespace b_net {
    // Server class that can handle request - check routes, queries and make response
    // Constructor: (usigned short port, int threads)
    class Server {
        const std::shared_ptr<asio::io_context> m_context;
        std::shared_ptr<Listener::Listener> m_listener;
        std::vector<std::thread> m_threadsArray;
        uint32_t m_threads;
        // routes that would be handled
        std::vector<b_net::Route> m_routes;

    public:
        Server (unsigned short port, int threads)
            : m_context(std::make_shared<asio::io_context>(threads)), m_threads(threads),
            m_listener(std::make_shared<Listener::Listener>(*m_context, tcp::endpoint{ tcp::v4(), port }, m_routes))
        {
            m_listener->run();
        }

        // Put new route to handle by the server
        void ROUTE (std::vector<Method> methods, std::string target, std::vector<Query> queries,
            std::function<void(Response&, std::string, std::list<ParsedQuery>&, Method)> handler)
        {
            m_routes.push_back(Route{ methods, target, queries, handler });
        }

        // Put new route to handle by the server
        void ROUTE (std::vector<Method> methods, std::string target,
            std::function<void(Response&, std::string, std::list<ParsedQuery>&, Method)> handler)
        {
            m_routes.push_back(Route{ methods, target, {}, handler });
        }

        void run()
        {
            // Run the I/O service on the requested number of threads
            m_threadsArray.reserve(m_threads - 1);
            for (auto i = m_threads - 1; i > 0; --i)
                m_threadsArray.emplace_back([this] { m_context->run(); });
            m_context->run();
        }
    };
}