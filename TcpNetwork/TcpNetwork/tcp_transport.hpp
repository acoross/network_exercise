//
//  tcp_transport.hpp
//  TcpNetwork
//
//  Created by 신일환 on 2016. 8. 18..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef tcp_transport_hpp
#define tcp_transport_hpp

#define ASIO_STANDALONE
#include "asio.hpp"

#include <deque>
#include <memory>
#include <functional>
#include <atomic>

#include "tcp_buffer.hpp"

namespace network {

	class TcpTransport : public std::enable_shared_from_this<TcpTransport> {
	public:
		using tcp = asio::ip::tcp;
		using ReadHandler = std::function<bool(TcpBuffer&)>;

		~TcpTransport();

		static std::shared_ptr<TcpTransport> Make(asio::io_service& ios, tcp::socket&& socket, ReadHandler handler) {

			std::shared_ptr<TcpTransport> ret;
			ret.reset(new TcpTransport(ios, std::move(socket), handler));

			return ret;
		}

		void Start();
		void Stop();
		void Send(char* data, int size);

	private:
		TcpTransport(asio::io_service& ios, tcp::socket&& socket, ReadHandler handler);

		// call this inside strand_
		void do_send();
		void on_send_end();
		void do_recv();

		void shutdown();

		std::deque<TcpBufferSP> pending_write_buffers_;
		TcpBufferSP read_buffer_;

		tcp::socket socket_;
		asio::strand strand_;

		ReadHandler read_handler_;
		std::atomic_flag running_;
	};
}


#endif /* tcp_transport_hpp */
