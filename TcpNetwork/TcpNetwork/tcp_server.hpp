//
//  tcp_server.hpp
//  TcpNetwork
//
//  Created by 신일환 on 2016. 8. 18..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef tcp_server_hpp
#define tcp_server_hpp

#define ASIO_STANDALONE
#include "asio.hpp"

#include <stdio.h>
#include <atomic>

#include "tcp_transport.hpp"

namespace network {

	class TcpServer {
	public:
		using tcp = asio::ip::tcp;

		TcpServer(asio::io_service& ios, int port)
		: ios_(ios), acceptor_(ios, tcp::endpoint(tcp::v4(), port)), socket_(ios_) {
		}

		~TcpServer() {
			Stop();
			acceptor_.close();
		}

		void Start() {
			if (!running_.test_and_set())
				do_accept();
		}

		void Stop() {
			acceptor_.cancel();
			running_.clear();
		}

	private:
		void do_accept() {
			acceptor_.async_accept(socket_,
								   [this](std::error_code ec) {
									   if (!ec) {
										   start_transport(std::move(socket_));
									   }

									   do_accept();
								   });
		}

		std::shared_ptr<TcpTransport> start_transport(tcp::socket&& socket){
			auto transport =
			TcpTransport::Make(ios_,
							   std::move(socket),
							   [](TcpBuffer& buffer)->bool {
								   return false;
							   });
			transport->Start();
			return transport;
		}

		std::atomic_flag running_;
		asio::io_service& ios_;
		tcp::socket socket_;
		tcp::acceptor acceptor_;
	};
}

#endif /* tcp_server_hpp */
