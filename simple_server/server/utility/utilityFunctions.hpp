#pragma once
#include "../route/root_route.hpp"
#include "request_utility_funcs.hpp"
#include "response_utility_funcs.hpp"

namespace utility_ {
    //////////////////////////////////////////////////////////////////////
    // Append an HTTP rel-path to a local filesystem path.
    // The returned path is normalized for the platform.
    //////////////////////////////////////////////////////////////////////
    std::string path_cat(beast::string_view base, beast::string_view path)
    {
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
    void fail(beast::error_code ec, char const* what)
    {
        std::cerr << what << ": " << ec.message() << "\n";
    }

    //////////////////////////////////////////////////////////////////////
    // Validating a target
    //////////////////////////////////////////////////////////////////////
    bool isValid (const std::string target = "0")
    {
        if (target.find("..") != std::string::npos
            || target.find("//") != std::string::npos
            || target.at(0) != '/'
            || target.length() < 2
            || target.find("?") > target.find("&")
            || target.find("&&") != std::string::npos
            || target.find("??") != std::string::npos)
        { return false; }

        return true;
    }
}
