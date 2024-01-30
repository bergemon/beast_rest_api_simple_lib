#pragma once
#include "../sharedData/sharedData.hpp"

namespace beast = boost::beast;				// from <boost/beast.hpp>
namespace http = beast::http;				// from <boost/beast/http.hpp>
namespace asio = boost::asio;				// from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;			// from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;			// from <boost/asio/ip/tcp.hpp>
namespace filesystem = std::filesystem;		// filesystem
namespace websocket = beast::websocket;		// from <boost/beast/websocket.hpp>

namespace utility_ {

    //////////////////////////////////////////////////////////////////////
    // Return a reasonable mime type based on the extension of a file.
    //////////////////////////////////////////////////////////////////////
    inline beast::string_view mime_type(beast::string_view path) {
        using beast::iequals;
        auto const ext = [&path] {
            if (path.rfind(".") == beast::string_view::npos) {
                return beast::string_view{};
            }
            return path.substr(path.rfind("."));
        }();

        if (iequals(ext, ".htm")
            || iequals(ext, ".html")
            || iequals(ext, ".php")) { return "text/html"; }
        if (iequals(ext, ".tiff")
            || iequals(ext, ".tif")) { return "image/tiff"; }
        if (iequals(ext, ".svgz")
            || iequals(ext, ".svg")) { return "image/svg+xml"; }
        if (iequals(ext, ".jpe")
            || iequals(ext, ".jpg")
            || iequals(ext, ".jpeg")) { return "image/jpeg"; }
        if (iequals(ext, ".css")) { return "text/css"; }
        if (iequals(ext, ".txt")) { return "text/plain"; }
        if (iequals(ext, ".js")) { return "application/javascript"; }
        if (iequals(ext, ".json")) { return "application/json"; }
        if (iequals(ext, ".xml")) { return "application/xml"; }
        if (iequals(ext, ".swf")) { return "application/x-shockwave-flash"; }
        if (iequals(ext, ".flv")) { return "video/x-flv"; }
        if (iequals(ext, ".png")) { return "image/png"; }
        if (iequals(ext, ".gif")) { return "image/gif"; }
        if (iequals(ext, ".bmp")) { return "image/bmp"; }
        if (iequals(ext, ".ico")) { return "image/vnd.microsoft.icon"; }
        if (iequals(ext, ".ttf")) { return "font/ttf"; }
        if (iequals(ext, ".woff")) { return "font/woff"; }
        if (iequals(ext, ".woff2")) { return "text/woff2"; }
        return "application/text";
    }

    //////////////////////////////////////////////////////////////////////
    // Append an HTTP rel-path to a local filesystem path.
    // The returned path is normalized for the platform.
    //////////////////////////////////////////////////////////////////////
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

    //////////////////////////////////////////////////////////////////////
    // Report a failure
    //////////////////////////////////////////////////////////////////////
    inline void fail(beast::error_code ec, char const* what) {
        std::cerr << what << ": " << ec.message() << "\n";
    }

    //////////////////////////////////////////////////////////////////////
    // Convert beast request method to custom b_net::Method
    //////////////////////////////////////////////////////////////////////
    inline b_net::Method convertMethod(http::verb method) {
        using namespace b_net;

        switch(method) {
            case http::verb::head:
                return Method::HEAD;
            case http::verb::get:
                return Method::GET;
            case http::verb::post:
                return Method::POST;
            case http::verb::put:
                return Method::PUT;
            case http::verb::delete_:
                return Method::DELETE_;
            case http::verb::patch:
                return Method::PATCH;
            default:
                return Method::HEAD;
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Parse queries and it's values from request
    //////////////////////////////////////////////////////////////////////
    inline std::list<ParsedQuery> parseQueries (const std::string target) {
        std::list<ParsedQuery> queries;

        // If there is no query parameters
        if(target.find("?") == std::string::npos) { return {}; }

        // String with query parameters to parse
        std::string q_str{
            target.substr(
                target.find("?") + 1,
                target.length() - (target.find("?") + 1)
            )
        };

        while(q_str.find("&") != std::string::npos) {

            queries.push_back({
                q_str.substr(0, q_str.find("=")),
                q_str.substr(q_str.find("=") + 1, q_str.find("&") - (q_str.find("=") + 1))
            });
            q_str =
                q_str.substr(q_str.find("&") + 1, q_str.length() - (q_str.find("&") + 1));

            if (q_str.find("&") == std::string::npos) {
                queries.push_back({
                    q_str.substr(0, q_str.find("=")),
                    q_str.substr(q_str.find("=") + 1, q_str.length() - (q_str.find("=") + 1))
                });
            }
        }
        return queries;
    }
}
