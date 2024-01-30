#pragma once
#include "../sharedData/sharedData.hpp"

namespace utility_ {
    //////////////////////////////////////////////////////////////////////
    // Parse queries and it's values from request
    //////////////////////////////////////////////////////////////////////
    std::list<ParsedQuery>&
        parseQueries
        (const std::string target, std::list<ParsedQuery>& l)
    {
        // If there is no query parameters
        if(target.find("?") == std::string::npos) { l.clear(); return l; }

        // String with query parameters to parse
        std::string q_str{
            target.substr(
                target.find("?") + 1,
                target.length() - (target.find("?") + 1)
            )
        };

        while(q_str.find("&") != std::string::npos) 
        {
            l.push_back({
                q_str.substr(0, q_str.find("=")),
                q_str.substr(q_str.find("=") + 1, q_str.find("&") - (q_str.find("=") + 1))
            });
            q_str =
                q_str.substr(q_str.find("&") + 1, q_str.length() - (q_str.find("&") + 1));

            if (q_str.find("&") == std::string::npos) {
                l.push_back({
                    q_str.substr(0, q_str.find("=")),
                    q_str.substr(q_str.find("=") + 1, q_str.length() - (q_str.find("=") + 1))
                });
            }
        }
        return l;
    }

    //////////////////////////////////////////////////////////////////////
    // Convert beast request method to custom b_net::Method
    //////////////////////////////////////////////////////////////////////
    b_net::Method convertMethod(http::verb method)
    {
        using namespace b_net;

        switch(method)
        {
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
    // Parse cookies from the string
    //////////////////////////////////////////////////////////////////////
    template<bool isRequest, class Fields>
        std::string&
        get_cookie_field
        (http::header<isRequest, Fields>& header, std::string& str)
    {
        for (const auto& elem : header)
        {
            if (elem.name_string() == "Cookie")
                str = elem.value();
        }
        return str;
    }

    //////////////////////////////////////////////////////////////////////
    // Parse cookies from the string
    //////////////////////////////////////////////////////////////////////
        std::list<b_net::ParsedCookie>&
        parse_cookies
        (std::string cookies, std::list<b_net::ParsedCookie>& l)
    {
        while (cookies.find(";") != std::string::npos)
        {
            l.push_back({
                cookies.substr(0, cookies.find("=")),
                cookies.substr(
                    cookies.find("=") + 1,
                    cookies.find(";") - (cookies.find("=") + 1)
                )
            });
            cookies = cookies.substr(cookies.find(";") + 2);
        }
        l.push_back({
            cookies.substr(0, cookies.find("=")),
            cookies.substr(cookies.find("=") + 1)
        });
        return l;
    }
}