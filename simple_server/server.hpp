#pragma once
#include "server/HTTP/listener.hpp"

namespace b_net {
    // Server class that can handle request - check routes, queries and make response
    // Constructor: (usigned short port, int threads)
    using Listener::Listener;
    class Server {
        // Parsed config class that contains all information
        // that needs to initialize b_net server
        class ParsedConfig {
            uint16_t m_port;
            uint16_t m_threads;

            // Setters
            void set_port(uint16_t port) { m_port = port; }
            void set_threads(uint16_t threads) { m_threads = threads; }
        
            // Getters
            const uint16_t port() const { return m_port; }
            const uint16_t threads() const { return m_threads; }

            ParsedConfig() { }
            ParsedConfig(uint16_t port, uint16_t threads)
                : m_port(port), m_threads(threads) { }

            friend class Server;
        };
        // We can have only one instance of the server class
        static Server m_server;

        // config settings
        const ParsedConfig m_config;
        // other server class members
        const std::shared_ptr<asio::io_context> m_context;
        const std::shared_ptr<Listener> m_listener;
        std::vector<std::thread> m_threadsArray;
        const uint32_t m_threads;
        // root routes that would be handled
        std::list<RoutesContainer> m_routes;

        // Recursive function to find any routes with slug and without handler
        // or with slug and nested routes (routes with slug can't have subroutes)
        // This method throws an exception if something goes wrong
        void check_route_container(
            const std::list<RoutesContainer>& list
        ) const
        {
            // Iterate subroutes
            for (auto& route : list)
            {
                route.valid_slug();
                
                // If route has subroutes then we invoke this function again
                if (route.sub_routes().size())
                {
                    // Recursion here
                    check_route_container(route.sub_routes());
                }
            }
        }
        
        // Find server config file and create new if it is not exist
        void find_config()
        {
            std::filesystem::path p("b_net_config.json");
            if (!std::filesystem::exists(p.c_str()))
            {
                std::ofstream config("b_net_config.json", std::ios::out);
                if (config.is_open())
                {
                    config << "{\n" << "\"port\": 80,\n" << "\"threads\": 1\n" << "}";
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

        Server()
            : m_config(parse_config()),
            m_context(std::make_shared<asio::io_context>(m_config.threads())),
            m_threads(m_config.threads()),
            m_listener(std::make_shared<Listener>(*m_context, tcp::endpoint{ tcp::v4(), m_config.port() }, m_routes))
        { }

    public:
        Server(const Server&) = delete;
        void operator=(const Server&) = delete;

        // Getter
        static Server& get_server() { return m_server; }

        // Create new root route
        // Warning! This method returns reference to the created object. Do not copy it!
        [[nodiscard]] RoutesContainer& ROOT_ROUTE(const std::string target)
        {
            m_routes.push_back({ target, 0 });
            return m_routes.back();
        }
        
        // Create new root route that will be handle request by itself
        // With queries
        // Warning! This method returns reference to the created object. Do not copy it!
        [[nodiscard]] RoutesContainer& ROOT_ROUTE(
            const std::vector<method> methods,
            const std::string target,
            const std::vector<query> queries,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_routes.emplace_back(methods, target, queries, handler, 0);
            return m_routes.back();
        }

        // Create new root route that will be handle request by itself
        // Without queries
        // Warning! This method returns reference to the created object. Do not copy it!
        [[nodiscard]] RoutesContainer& ROOT_ROUTE(
            const std::vector<method> methods,
            const std::string target,
            const std::function<void(Request&, Response&)> handler
        )
        {
            m_routes.emplace_back(methods, target, handler, 0);
            return m_routes.back();
        }

        // Run the server, start to listen incoming messages on the setted port
        void run()
        {
            try {
                check_route_container(m_routes);
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                return;
            }

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
b_net::Server b_net::Server::m_server;