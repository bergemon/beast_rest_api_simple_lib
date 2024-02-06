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
#include <array>
#include <cassert>
#include <list>
#include <filesystem>
#include <fstream>
#include <optional>
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

    // Response body type
    enum BodyType {
        TEXT, JSON, JPEG, PNG, BMP, GIF, ZIP, RAR,
        HTML, CSS, JS, TIFF, WEBP, AVI, MP4, ICO,
        XML, WOFF, WOFF2, SVG, SEVEN_ZIP, MP3, MPEG,
        BINARY
    };
    
    // Class container for parsed queries
    class ParsedQuery final {
        const std::string m_query;
        const std::string m_value;
        
    public:
        ParsedQuery(std::string query, std::string value)
            : m_query(query), m_value(value) {}
        // Getters
        std::string query() const { return m_query; }
        std::string value() const { return m_value; }
    };

    // Class container for parsed cookies
    class ParsedCookie final {
        const std::string m_name;
        const std::string m_value;

    public:
        ParsedCookie(std::string name, std::string value)
            : m_name(name), m_value(value) {}
        // Getters
        std::string name() const { return m_name; }
        std::string value() const { return m_value; }
    };
}

// Body type
using b_net::BodyType::TEXT;
using b_net::BodyType::JSON;
using b_net::BodyType::JPEG;
using b_net::BodyType::PNG;
using b_net::BodyType::BMP;
using b_net::BodyType::GIF;
using b_net::BodyType::ZIP;
using b_net::BodyType::RAR;
using b_net::BodyType::HTML;
using b_net::BodyType::CSS;
using b_net::BodyType::JS;
using b_net::BodyType::TIFF;
using b_net::BodyType::WEBP;
using b_net::BodyType::AVI;
using b_net::BodyType::MP4;
using b_net::BodyType::ICO;
using b_net::BodyType::XML;
using b_net::BodyType::WOFF;
using b_net::BodyType::WOFF2;
using b_net::BodyType::SVG;
using b_net::BodyType::SEVEN_ZIP;
using b_net::BodyType::MP3;
using b_net::BodyType::MPEG;
using b_net::BodyType::BINARY;

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