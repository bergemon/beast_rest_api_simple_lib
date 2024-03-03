#pragma once
#include "session.hpp"

namespace Listener {
    class Listener : public std::enable_shared_from_this<Listener> {
        asio::io_context& m_context;
        tcp::acceptor m_acceptor;
        std::list<b_net::RoutesContainer>& m_routes;

    public:
        Listener(asio::io_context& context, tcp::endpoint endpoint, std::list<b_net::RoutesContainer>& root_routes)
            : m_context(context), m_acceptor(asio::make_strand(m_context)), m_routes(root_routes)
        {
            beast::error_code ec;

            m_acceptor.open(endpoint.protocol(), ec);
            if (ec) {
                utility_::fail(ec, "open");
                return;
            }

            m_acceptor.set_option(asio::socket_base::reuse_address(true), ec);
            if (ec) {
                utility_::fail(ec, "set_option");
                return;
            }

            m_acceptor.bind(endpoint, ec);
            if (ec) {
                utility_::fail(ec, "bind");
                return;
            }

            m_acceptor.listen(asio::socket_base::max_listen_connections, ec);
            if (ec) {
                utility_::fail(ec, "listen");
                return;
            }
        } // End of constructor

        void run() { do_accept(); }

    private:
        void do_accept()
        {
            m_acceptor.async_accept(asio::make_strand(m_context),
                beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
        }

        void on_accept(beast::error_code ec, tcp::socket socket)
        {
            if (ec)
            {
                utility_::fail(ec, "accept");
                return;
            }
            else
            {
                std::make_shared<Session::Session>(std::move(socket), m_routes)->run();
            }

            do_accept();
        }
    };
}