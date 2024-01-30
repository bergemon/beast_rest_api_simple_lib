#pragma once
#include "../route/route.hpp"
#include "request_utility_funcs.hpp"
#include "response_utility_funcs.hpp"

namespace utility_ {
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
}
