#pragma once
#include "../handleRequest/handleRequest.hpp"

namespace Session {
    // Handles an HTTP server connection
    class Session : public std::enable_shared_from_this<Session> {
        beast::tcp_stream m_stream;
        beast::flat_buffer m_buffer;
        http::request<http::string_body> m_request;
        std::vector<b_net::Route>& m_routes;
        b_net::Response m_custom_response;

    public:
        Session(tcp::socket&& socket, std::vector<b_net::Route>& routes)
            : m_stream(std::move(socket)), m_routes(routes) { }

        ~Session() { m_custom_response.clear(); }

        void run()
        {
            asio::dispatch(m_stream.get_executor(),
                beast::bind_front_handler(&Session::do_read, shared_from_this()));
        }

        void do_read()
        {
            m_request = {};
            m_stream.expires_after(std::chrono::seconds(30));

            http::async_read(m_stream, m_buffer, m_request,
                beast::bind_front_handler(&Session::on_read, shared_from_this()));
        }

        void on_read(beast::error_code ec, std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);

            if (ec == http::error::end_of_stream)
                return do_close();

            if (ec)
                return utility_::fail(ec, "read");

            send_response(
                HandleRequest::handle_request(
                    m_custom_response,
                    std::move(m_request),
                    m_routes
                )
            );
        }

        void send_response(http::message_generator&& msg)
        {
            bool keep_alive = msg.keep_alive();

            beast::async_write(m_stream, std::move(msg),
                beast::bind_front_handler(&Session::on_write, shared_from_this(), keep_alive));
        }

        void on_write(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);

            if (ec)
                return utility_::fail(ec, "write");

            if (!keep_alive)
                return do_close();
                
            do_read();
        }

        void do_close()
        {
            beast::error_code ec;
            m_stream.socket().shutdown(tcp::socket::shutdown_send, ec);
        }

        // Custom response class with data to transmit
        b_net::Response& get_response()
        {
            return m_custom_response;
        }
    };
}