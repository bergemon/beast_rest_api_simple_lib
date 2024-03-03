#pragma once
#include "sharedData.hpp"

// Body type
using b_net::body_type::TEXT;
using b_net::body_type::JSON;
using b_net::body_type::JPEG;
using b_net::body_type::PNG;
using b_net::body_type::BMP;
using b_net::body_type::GIF;
using b_net::body_type::ZIP;
using b_net::body_type::RAR;
using b_net::body_type::HTML;
using b_net::body_type::CSS;
using b_net::body_type::JS;
using b_net::body_type::TIFF;
using b_net::body_type::WEBP;
using b_net::body_type::AVI;
using b_net::body_type::MP4;
using b_net::body_type::ICO;
using b_net::body_type::XML;
using b_net::body_type::WOFF;
using b_net::body_type::WOFF2;
using b_net::body_type::SVG;
using b_net::body_type::SEVEN_ZIP;
using b_net::body_type::MP3;
using b_net::body_type::MPEG;
using b_net::body_type::BINARY;

// Type of query
using b_net::query_type::BOOL_;
using b_net::query_type::INT_;
using b_net::query_type::STR_;

// Methods
using b_net::method::ALL;
using b_net::method::HEAD;
using b_net::method::GET;
using b_net::method::POST;
using b_net::method::PUT;
using b_net::method::DELETE_;
using b_net::method::PATCH;

// Parameters for handler prototype
using b_net::ParsedField;

// Error code custom class
using b_net::status;

// Slug types
using b_net::slug_type::NO_SLUG;
using b_net::slug_type::INT_SLUG;
using b_net::slug_type::STR_SLUG;