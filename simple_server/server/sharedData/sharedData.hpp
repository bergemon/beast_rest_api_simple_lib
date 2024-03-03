#pragma once
#include "error_code.hpp"

// Utility functions forward declaration
namespace b_net {
    class ParsedField;
}

// Utility functions forward declaration
namespace utility_ {
    using namespace b_net;

    template<bool isRequest, class Fields>
    std::list<ParsedField>& get_req_header_fields
    (http::header<isRequest, Fields>& header, std::list<ParsedField>& list);

    std::list<ParsedField>& parse_queries(const std::string target, std::list<ParsedField>& list);

    std::list<ParsedField>& parse_cookies
        (std::list<ParsedField>& header, std::list<ParsedField>& list);
}

namespace b_net {
    // Type of query parameter
    enum query_type { STR_, INT_, BOOL_ };

    // Custom methods
    enum method { ALL, HEAD, GET, POST, PUT, DELETE_, PATCH };

    // Response body type
    enum body_type {
        TEXT, JSON, JPEG, PNG, BMP, GIF, ZIP, RAR,
        HTML, CSS, JS, TIFF, WEBP, AVI, MP4, ICO,
        XML, WOFF, WOFF2, SVG, SEVEN_ZIP, MP3, MPEG,
        BINARY
    };

    // Slug type
    enum slug_type {
        NO_SLUG,
        INT_SLUG,
        STR_SLUG
    };

    // Class container for parsed data that has only
    // two fields name and value
    class ParsedField final {
    protected:
        const std::string m_name;
        const std::string m_value;

        ParsedField(const std::string name, const std::string value)
            : m_name(name), m_value(value) { }

        // Friendly utility functions
        template<bool isRequest, class Fields>
        friend std::list<ParsedField>& utility_::get_req_header_fields
        (http::header<isRequest, Fields>& header, std::list<ParsedField>& list);

        friend std::list<ParsedField>& utility_::parse_queries
            (const std::string target, std::list<ParsedField>& list);
        
        friend std::list<ParsedField>& utility_::parse_cookies
            (std::list<ParsedField>& header, std::list<ParsedField>& list);

    public:
        // Name of the parsed row/field
        const std::string name() const { return m_name; }
        // Value of the parsed row/field
        const std::string value() const { return m_value; }
    };
}