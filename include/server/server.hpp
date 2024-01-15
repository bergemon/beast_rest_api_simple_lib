#pragma once
#include "../HTTP/listener.hpp"

namespace bergemon {
    // Server class that can handle request - check routes, queries and make response
    // Constructor: (usigned short port, int threads)
    class Server {
        const std::shared_ptr<asio::io_context> m_context;
        Listener::Listener m_listener;
        std::vector<std::thread> m_threadsArray;
        // routes that would be handled
        std::vector<bergemon::Route> m_routes;

    public:
        Server (unsigned short port, int threads)
            : m_context(std::move(std::make_shared<asio::io_context>())),
            m_listener(*m_context, tcp::endpoint{ tcp::v4(), port }, m_routes)
        {
            m_listener.run();

            // Run the I/O service on the requested number of threads
            m_threadsArray.reserve(threads - 1);
            for (auto i = threads - 1; i > 0; --i)
                m_threadsArray.emplace_back([this] { m_context->run(); });
            m_context->run();
        }

        // Put new route to handle by the server
        void ROUTE (std::vector<Method> methods, std::string target, std::vector<Query> queries,
            std::function<http::message_generator(const uint32_t, const bool)> handler)
        {
            m_routes.push_back(Route{ methods, target, queries, handler });
        }
    };
}