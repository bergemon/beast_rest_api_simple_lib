#pragma once
#include "../sharedData/sharedData.hpp"

namespace utility_ {
    //////////////////////////////////////////////////////////////////////
    // Get http request header fields from Boost Beast
    // to b_net ParsedField class
    //////////////////////////////////////////////////////////////////////
    template<bool isRequest, class Fields>
    std::list<ParsedField>&
        get_req_header_fields
        (http::header<isRequest, Fields>& header, std::list<ParsedField>& list)
    {
        for (const auto& elem : header)
        {
            list.push_back({elem.name_string(), elem.value()});
        }

        return list;
    }
    //////////////////////////////////////////////////////////////////////
    // Parse queries and it's values from request
    //////////////////////////////////////////////////////////////////////
    std::list<ParsedField>&
        parse_queries
        (const std::string target, std::list<ParsedField>& list)
    {
        // If there is no query parameters
        if(target.find("?") == std::string::npos) { list.clear(); return list; }

        // String with query parameters to parse
        std::string q_str{
            target.substr(
                target.find("?") + 1,
                target.length() - (target.find("?") + 1)
            )
        };

        while(q_str.find("&") != std::string::npos) 
        {
            list.push_back({
                q_str.substr(0, q_str.find("=")),
                q_str.substr(q_str.find("=") + 1, q_str.find("&") - (q_str.find("=") + 1))
            });
            q_str =
                q_str.substr(q_str.find("&") + 1, q_str.length() - (q_str.find("&") + 1));

            if (q_str.find("&") == std::string::npos) {
                list.push_back({
                    q_str.substr(0, q_str.find("=")),
                    q_str.substr(q_str.find("=") + 1, q_str.length() - (q_str.find("=") + 1))
                });
            }
        }
        return list;
    }

    //////////////////////////////////////////////////////////////////////
    // Convert beast request method to custom b_net::Method
    //////////////////////////////////////////////////////////////////////
    b_net::method convert_method(http::verb method)
    {
        using namespace b_net;

        switch(method)
        {
            case http::verb::head: return method::HEAD;
            case http::verb::get: return method::GET;
            case http::verb::post: return method::POST;
            case http::verb::put: return method::PUT;
            case http::verb::delete_: return method::DELETE_;
            case http::verb::patch: return method::PATCH;
            default: return method::HEAD;
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Parse cookies from the string
    //////////////////////////////////////////////////////////////////////
    std::string
        get_cookie_field
        (std::list<ParsedField>& header)
    {
        std::string str;
        for (const auto& elem : header)
        {
            if (string_to_lower(elem.name()) == "cookie")
                str = elem.value();
        }
        return str;
    }

    //////////////////////////////////////////////////////////////////////
    // Parse cookies from the string
    //////////////////////////////////////////////////////////////////////
    std::list<ParsedField>&
        parse_cookies
        (
            std::list<ParsedField>& header,
            std::list<ParsedField>& list
        )
    {
        std::string cookies{ get_cookie_field(header) };

        while (cookies.find(";") != std::string::npos)
        {
            list.push_back({
                cookies.substr(0, cookies.find("=")),
                cookies.substr(
                    cookies.find("=") + 1,
                    cookies.find(";") - (cookies.find("=") + 1)
                )
            });
            cookies = cookies.substr(cookies.find(";") + 2);
        }
        list.push_back({
            cookies.substr(0, cookies.find("=")),
            cookies.substr(cookies.find("=") + 1)
        });
        return list;
    }
    //////////////////////////////////////////////////////////////////////
    // Mime type to b_net BodyType
    //////////////////////////////////////////////////////////////////////
    b_net::body_type mimeType_to_bodyType(std::string mime_type)
    {
        using namespace b_net;

        if(mime_type == "text/plain") return TEXT;
        if(mime_type == "application/json") return JSON;
        if(mime_type == "image/jpeg") return JPEG;
        if(mime_type == "image/png") return PNG;
        if(mime_type == "image/bmp") return BMP;
        if(mime_type == "image/gif") return GIF;
        if(mime_type == "application/zip") return ZIP;
        if(mime_type == "application/vnd.rar") return RAR;
        if(mime_type == "text/html") return HTML;
        if(mime_type == "text/css") return CSS;
        if(mime_type == "text/javascript") return JS;
        if(mime_type == "image/tiff") return TIFF;
        if(mime_type == "image/webp") return WEBP;
        if(mime_type == "video/x-msvideo") return AVI;
        if(mime_type == "video/mp4") return MP4;
        if(mime_type == "image/vnd.microsoft.icon") return ICO;
        if(mime_type == "application/xml") return XML;
        if(mime_type == "font/woff") return WOFF;
        if(mime_type == "font/woff2") return WOFF2;
        if(mime_type == "image/svg+xml") return SVG;
        if(mime_type == "application/x-7z-compressed") return SEVEN_ZIP;
        if(mime_type == "audio/mpeg") return MP3;
        if(mime_type == "video/mpeg") return MPEG;

        return BINARY;
    }
    //////////////////////////////////////////////////////////////////////
    // Parse http request header fields to get body type
    //////////////////////////////////////////////////////////////////////
    template<bool isRequest, class Fields>
    std::string parse_bodyType(http::header<isRequest, Fields>& header)
    {
        std::string content_type;
        for (const auto& elem : header)
        {
            std::string field = elem.name_string();
            if (string_to_lower(field) == "content-type")
                content_type = elem.value();
        }
        return content_type;
    }
}