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
        if (iequals(ext, ".avi")) { return "video/x-msvideo"; }
        if (iequals(ext, ".mp3")) { return "audio/mpeg"; }
        if (iequals(ext, ".mp4")) { return "video/mp4"; }
        if (iequals(ext, ".mpeg")) { return "video/mpeg"; }
        if (iequals(ext, ".7z")) { return "application/x-7z-compressed"; }
        if (iequals(ext, ".rar")) { return "application/x-rar-compressed"; }
        if (iequals(ext, ".zip")) { return "application/x-zip-compressed"; }
        if (iequals(ext, ".css")) { return "text/css"; }
        if (iequals(ext, ".txt")) { return "text/plain"; }
        if (iequals(ext, ".js")) { return "application/javascript"; }
        if (iequals(ext, ".json")) { return "application/json"; }
        if (iequals(ext, ".xml")) { return "application/xml"; }
        if (iequals(ext, ".swf")) { return "application/x-shockwave-flash"; }
        if (iequals(ext, ".png")) { return "image/png"; }
        if (iequals(ext, ".gif")) { return "image/gif"; }
        if (iequals(ext, ".bmp")) { return "image/bmp"; }
        if (iequals(ext, ".ico")) { return "image/vnd.microsoft.icon"; }
        if (iequals(ext, ".ttf")) { return "font/ttf"; }
        if (iequals(ext, ".woff")) { return "font/woff"; }
        if (iequals(ext, ".woff2")) { return "text/woff2"; }

        return "application/octet-stream";
    }

    //////////////////////////////////////////////////////////////////////
    // This function is coverting b_net::BodyType to string mime_type
    //////////////////////////////////////////////////////////////////////
    inline std::string bodyType_to_mimeType(b_net::BodyType type) {
        using namespace b_net;

        switch(type)
        {
            case TEXT: return {".txt"};
            case JSON: return {".json"};
            case JPEG: return {".jpeg"};
            case PNG: return {".png"};
            case BMP: return {".bmp"};
            case GIF: return {".gif"};
            case ZIP: return {".zip"};
            case RAR: return {".rar"};
            case HTML: return {".html"};
            case CSS: return {".css"};
            case JS: return {".js"};
            case TIFF: return {".tiff"};
            case WEBP: return {".webp"};
            case AVI: return {".avi"};
            case MP4: return {".mp4"};
            case ICO: return {".ico"};
            case XML: return {".xml"};
            case WOFF: return {".woff"};
            case WOFF2: return {".woff2"};
            case SVG: return {".svg"};
            case SEVEN_ZIP: return {".7z"};
            case MP3: return {".mp3"};
            case MPEG: return {".mpeg"};
            case BINARY: return {""};
            default: return {""};
        }
        return {""};
    }

    // Get copy of inputed string but with all characters in lower case
    // You can not put string with length more that 500 characters
    inline std::string string_to_lower(std::string& str)
    {
        assert(str.length() <= 500);

        std::string temp_str;
        for (auto& elem : str)
            temp_str.push_back(std::tolower(elem));

        return temp_str;
    }
}