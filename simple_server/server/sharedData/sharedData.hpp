#pragma once
#include "error_code.hpp"

#ifdef _WIN32
    #define _WIN32_WINNT 0x0601
#endif

// List of core dependencies
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <list>
#include <filesystem>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace beast = boost::beast;				// from <boost/beast.hpp>
namespace http = beast::http;				// from <boost/beast/http.hpp>
namespace asio = boost::asio;				// from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;			// from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;			// from <boost/asio/ip/tcp.hpp>
namespace filesystem = std::filesystem;		// filesystem
namespace websocket = beast::websocket;		// from <boost/beast/websocket.hpp>

namespace b_net {
    // Type of query parameter
    enum Type { STR_, INT_, BOOL_ };

    // Custom methods
    enum Method { ALL, HEAD, GET, POST, PUT, DELETE_, PATCH };
    
    // Class container for parsed queries
    class ParsedQuery {
        const std::string m_query;
        const std::string m_value;
        
    public:
        ParsedQuery(std::string query, std::string value)
            : m_query(query), m_value(value) {}
        std::string query() const { return m_query; }
        std::string value() const { return m_value; }
    };

    // Class container for parsed cookies
    class ParsedCookie {
        const std::string m_name;
        const std::string m_value;

    public:
        ParsedCookie(std::string name, std::string value)
            : m_name(name), m_value(value) {}
        std::string name() const { return m_name; }
        std::string value() const { return m_value; }
    };
}

// Type of query
using b_net::Type::BOOL_;
using b_net::Type::INT_;
using b_net::Type::STR_;

// Methods
using b_net::Method::ALL;
using b_net::Method::HEAD;
using b_net::Method::GET;
using b_net::Method::POST;
using b_net::Method::PUT;
using b_net::Method::DELETE_;
using b_net::Method::PATCH;

// Parameters for handler prototype
using b_net::ParsedQuery;