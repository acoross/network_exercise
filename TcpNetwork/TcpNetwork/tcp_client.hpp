//
//  tcp_client.hpp
//  TcpNetwork
//
//  Created by 신일환 on 2016. 8. 18..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef tcp_client_hpp
#define tcp_client_hpp

#define ASIO_STANDALONE
#include "asio.hpp"

#include <iostream>
#include <memory>

#include "tcp_transport.hpp"
#include "log.hpp"

namespace network {
	class TcpClient {
	public:
		using tcp = asio::ip::tcp;

		TcpClient(asio::io_service& ios) : ios_(ios), socket_(ios) {
		}
		
		void Connect(const char* const host, const char* const port) {

			tcp::resolver resolver(ios_);
			auto endpoint_iterator = resolver.resolve({ host, port });

			asio::async_connect(socket_, endpoint_iterator,
								[this](std::error_code ec, tcp::resolver::iterator) {
									if (!ec)
									{
										if (transport_){
											transport_->Stop();
										}

										transport_ = start_transport(std::move(socket_));
									} else {
										util::Logger::Log("connection failed\n");
									}
								});
		}

	private:
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

		asio::io_service& ios_;
		tcp::socket socket_;
		std::shared_ptr<TcpTransport> transport_;
	};
}

#endif /* tcp_client_hpp */
