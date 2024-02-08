#pragma once
#include "sharedData.hpp"

// Body type
using b_net::BodyType::TEXT;
using b_net::BodyType::JSON;
using b_net::BodyType::JPEG;
using b_net::BodyType::PNG;
using b_net::BodyType::BMP;
using b_net::BodyType::GIF;
using b_net::BodyType::ZIP;
using b_net::BodyType::RAR;
using b_net::BodyType::HTML;
using b_net::BodyType::CSS;
using b_net::BodyType::JS;
using b_net::BodyType::TIFF;
using b_net::BodyType::WEBP;
using b_net::BodyType::AVI;
using b_net::BodyType::MP4;
using b_net::BodyType::ICO;
using b_net::BodyType::XML;
using b_net::BodyType::WOFF;
using b_net::BodyType::WOFF2;
using b_net::BodyType::SVG;
using b_net::BodyType::SEVEN_ZIP;
using b_net::BodyType::MP3;
using b_net::BodyType::MPEG;
using b_net::BodyType::BINARY;

// Type of query
using b_net::Type::BOOL_;
using b_net::Type::INT_;
using b_net::Type::STR_;

// Methods
using b_net::Method::ALL;
using b_net::Method::HEAD;
using b_net::Method::GET;
using b_net::Method::POST;
using b_net::Method::PUT;
using b_net::Method::DELETE_;
using b_net::Method::PATCH;

// Parameters for handler prototype
using b_net::ParsedField;