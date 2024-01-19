#pragma once
#include "../responseCreator/responseCreator.hpp"
#include "errorCodes.hpp"

namespace HandleRequest {
    template <class Body, class Allocator>
    http::message_generator handle_request (
        http::request<Body, http::basic_fields<Allocator>>&& req,
        std::vector<bergemon::Route>& routes
    )
    {
        for (auto& elem : routes) {
            using namespace bergemon;

            if (!elem.isValid(req.target()))
                return ErrorCodes::bad_request(req.version(), req.keep_alive(), "Illegal request-target");

            if (!elem.isTarget(req.target()))
                continue;

            if (!elem.methodAllowed(req.method()))
                return ErrorCodes::bad_request(req.version(), req.keep_alive(), "Unknown HTTP-method");

            std::list<ParsedQuery> queries_list = elem.parseQueries(req.target());

            if(!elem.queriesExist(queries_list))
                return ErrorCodes::bad_request(req.version(), req.keep_alive(), "Not allowed query parameter");

            bergemon::Response custom_res(
                elem.handler()(
                    req.target(),
                    queries_list,
                    convertMethod(req.method())
                )
            );

            return createResponse(custom_res, req.version(), req.keep_alive());
        }

        // If there is no such request target
        return ErrorCodes::not_found(req.version(), req.keep_alive(), req.target());

        // Build the path to the requested file
        // std::string path = path_cat(".", req.target());
        // if (req.target().back() == '/')
        //     path.append("index.html");

        // Attempt to open the file
        // beast::error_code ec;
        // http::file_body::value_type body;
        // body.open(path.c_str(), beast::file_mode::scan, ec);

        // Handle an unknown error
        // if(ec)
        //     return ErrorCodes::server_error(req.version(), req.keep_alive(), ec.message());

        // Cache the size since we need it after the move
        // auto const size = body.size();
    }
}