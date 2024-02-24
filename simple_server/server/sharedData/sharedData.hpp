#pragma once
#include "error_code.hpp"

namespace b_net {
    // Type of query parameter
    enum Type { STR_, INT_, BOOL_ };

    // Custom methods
    enum Method { ALL, HEAD, GET, POST, PUT, DELETE_, PATCH };

    // Response body type
    enum BodyType {
        TEXT, JSON, JPEG, PNG, BMP, GIF, ZIP, RAR,
        HTML, CSS, JS, TIFF, WEBP, AVI, MP4, ICO,
        XML, WOFF, WOFF2, SVG, SEVEN_ZIP, MP3, MPEG,
        BINARY
    };

    // Slug type
    enum SlugType {
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

    public:
        ParsedField(const std::string name, const std::string value)
            : m_name(name), m_value(value) { }

        // Name of the parsed row/field
        const std::string name() const { return m_name; }
        // Value of the parsed row/field
        const std::string value() const { return m_value; }
    };
}