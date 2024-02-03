#pragma once
#include "server/HTTP/listener.hpp"

namespace b_net {
    // Parsed config class that contains all information
    // that needs to initialize b_net server
    class ParsedConfig {
        uint16_t m_port;
        uint16_t m_threads;

    public:
        ParsedConfig() { }
        ParsedConfig(uint16_t port, uint16_t threads)
            : m_port(port), m_threads(threads) { }

        // Setters
        void set_port(uint16_t port) { m_port = port; }
        void set_threads(uint16_t threads) { m_threads = threads; }
        
        // Getters
        uint16_t port() { return m_port; }
        uint16_t threads() { return m_threads; }
    };

    // Server class that can handle request - check routes, queries and make response
    // Constructor: (usigned short port, int threads)
    using Listener::Listener;
    class Server {
        // config settings
        ParsedConfig m_config;
        // other server class members
        const std::shared_ptr<asio::io_context> m_context;
        std::shared_ptr<Listener> m_listener;
        std::vector<std::thread> m_threadsArray;
        uint32_t m_threads;
        // routes that would be handled
        std::vector<b_net::Route> m_routes;

    public:
        Server()
            : m_config(parse_config()),
            m_context(std::make_shared<asio::io_context>(m_config.threads())),
            m_threads(m_config.threads()),
            m_listener(std::make_shared<Listener>(*m_context, tcp::endpoint{ tcp::v4(), m_config.port() }, m_routes))
        { }

        // Find server config file and create new if it is not exist
        void find_config()
        {
            std::filesystem::path p("b_net_config.json");
            if (!std::filesystem::exists(p.c_str()))
            {
                std::ofstream config("b_net_config.json", std::ios::out);
                if (config.is_open())
                {
                    config << "{\n"
                        << "    \"port\": 80,\n"
                        << "    \"threads\": 1\n"
                        << "}";
                }
                config.close();
            }
        }

        // Parse config file
        ParsedConfig parse_config()
        {
            find_config();
            ParsedConfig config_;
            std::ifstream config("b_net_config.json", std::ios::in);
            std::string line;

            while(getline(config, line))
            {
                if (
                    line.find("{") != std::string::npos
                    || line.find("}") != std::string::npos
                )
                    continue;

                if (line.find("\"port\"") != std::string::npos)
                    config_.set_port(
                        std::atoi(line.substr(line.find(":") + 1).c_str())
                    );

                if (line.find("\"threads\"") != std::string::npos)
                    config_.set_threads(
                        std::atoi(line.substr(line.find(":") + 1).c_str())
                    );
            }

            return config_;
        }

        // Put new route to handle by the server
        void ROUTE (const std::vector<Method> methods,
            const std::string target,
            const std::vector<Query> queries,
            const std::function<void(Request&, Response&)> handler)
        {
            m_routes.push_back(Route{ methods, target, queries, handler });
        }

        // Put new route to handle by the server
        void ROUTE (const std::vector<Method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler)
        {
            m_routes.push_back(Route{ methods, target, {}, handler });
        }

        // Run the server, start to listen incoming messages on the setted port
        void run()
        {
            // Start to listen incoming connections
            m_listener->run();

            // Run the I/O service on the requested number of threads
            m_threadsArray.reserve(m_threads - 1);
            for (auto i = m_threads - 1; i > 0; --i)
                m_threadsArray.emplace_back([this] { m_context->run(); });
            m_context->run();
        }
    };
}