#pragma once
#include "sharedData.hpp"

namespace utility_ {
    //////////////////////////////////////////////////////////////////////
    // Return a reasonable mime type based on the extension of a file.
    //////////////////////////////////////////////////////////////////////
    inline beast::string_view mime_type(beast::string_view path)
    {
        using beast::iequals;
        auto const ext = [&path] {
            if (path.rfind(".") == beast::string_view::npos) {
                return beast::string_view{};
            }
            return path.substr(path.rfind("."));
        }();

        // File extension
        if (iequals(ext, ".htm") || iequals(ext, ".html") || iequals(ext, ".php"))
        {
            return "text/html";
        }
        if (iequals(ext, ".jpe") || iequals(ext, ".jpg") || iequals(ext, ".jpeg"))
        {
            return "image/jpeg";
        }
        if (iequals(ext, ".tiff") || iequals(ext, ".tif"))
        {
            return "image/tiff";
        }
        if (iequals(ext, ".svgz") || iequals(ext, ".svg"))
        {
            return "image/svg+xml";
        }
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
}