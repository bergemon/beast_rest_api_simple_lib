#pragma once

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