#pragma once

#ifdef _WIN32
    #define _WIN32_WINNT 0x0601
#endif

// List of core dependencies
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <list>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>
#include <boost/optional.hpp>

namespace beast = boost::beast;				// from <boost/beast.hpp>
namespace http = beast::http;				// from <boost/beast/http.hpp>
namespace asio = boost::asio;				// from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;			// from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;			// from <boost/asio/ip/tcp.hpp>
namespace filesystem = std::filesystem;		// filesystem
namespace websocket = beast::websocket;		// from <boost/beast/websocket.hpp>

// Return a reasonable mime type based on the extension of a file.
inline beast::string_view mime_type(beast::string_view path) {
    using beast::iequals;
    auto const ext = [&path] {
            auto const pos = path.rfind(".");
            if (pos == beast::string_view::npos) {
                return beast::string_view{};
            }
            return path.substr(pos);
    }();

    if (iequals(ext, ".htm"))  return "text/html";
    if (iequals(ext, ".html")) return "text/html";
    if (iequals(ext, ".php"))  return "text/html";
    if (iequals(ext, ".css"))  return "text/css";
    if (iequals(ext, ".txt"))  return "text/plain";
    if (iequals(ext, ".js"))   return "application/javascript";
    if (iequals(ext, ".json")) return "application/json";
    if (iequals(ext, ".xml"))  return "application/xml";
    if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if (iequals(ext, ".flv"))  return "video/x-flv";
    if (iequals(ext, ".png"))  return "image/png";
    if (iequals(ext, ".jpe"))  return "image/jpeg";
    if (iequals(ext, ".jpeg")) return "image/jpeg";
    if (iequals(ext, ".jpg"))  return "image/jpeg";
    if (iequals(ext, ".gif"))  return "image/gif";
    if (iequals(ext, ".bmp"))  return "image/bmp";
    if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff")) return "image/tiff";
    if (iequals(ext, ".tif"))  return "image/tiff";
    if (iequals(ext, ".svg"))  return "image/svg+xml";
    if (iequals(ext, ".svgz")) return "image/svg+xml";
    if (iequals(ext, ".ttf")) return "font/ttf";
    if (iequals(ext, ".woff")) return "font/woff";
    if (iequals(ext, ".woff2")) return "text/woff2";
    return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
inline std::string path_cat(beast::string_view base, beast::string_view path) {
    if (base.empty())
        return std::string(path);
    std::string result(base);
#ifdef _WIN32
    char constexpr path_separator = '\\';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for (auto& c : result)
        if (c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}

// Report a failure
inline void fail(beast::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}