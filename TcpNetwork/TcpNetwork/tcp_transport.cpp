//
//  tcp_transport.cpp
//  TcpNetwork
//
//  Created by 신일환 on 2016. 8. 18..
//  Copyright © 2016년 acoross. All rights reserved.
//

#include <stdio.h>

#include "tcp_transport.hpp"

namespace network {

	TcpTransport::TcpTransport(asio::io_service& ios, tcp::socket&& socket, ReadHandler handler)
	: socket_(std::move(socket)), strand_(ios), read_handler_(handler) {

	}

	TcpTransport::~TcpTransport() {
		socket_.close();
	}

	void TcpTransport::Start() {
		if (!running_.test_and_set()) {
			do_recv();
		}
	}

	void TcpTransport::Stop() {
		shutdown();
	}

	void TcpTransport::Send(char* data, int size){
		strand_.dispatch([this, data, size](){
			bool has_pending = !pending_write_buffers_.empty();

			auto new_buffer = std::make_shared<TcpBuffer>();
			new_buffer->write(data, size);
			pending_write_buffers_.push_back(new_buffer);

			if (has_pending)
				do_send();
		});
	}

	///////////////////////////////////////////////////
	void TcpTransport::do_send() {
		auto self = shared_from_this();

		socket_.async_send(asio::buffer(pending_write_buffers_.front()->raw(), pending_write_buffers_.front()->data_size()),
						   [this, self](asio::error_code ec, size_t bytes_transferred){

							   if (ec)
								   shutdown();
							   else
								   on_send_end();

						   });
	}

	void TcpTransport::on_send_end() {
		auto self = shared_from_this();

		strand_.dispatch([this, self](){
			pending_write_buffers_.pop_front();
			if (!pending_write_buffers_.empty()) {
				do_send();
			}
		});
	}

	void TcpTransport::do_recv() {
		auto self = shared_from_this();

		socket_.async_read_some(
								asio::buffer(read_buffer_->empty_buffer(), read_buffer_->space()),
								[this, self](asio::error_code ec, size_t bytes_transferred){
									if (ec || !read_handler_(*read_buffer_))
										shutdown();
									else
										do_recv();
								});
	}

	void TcpTransport::shutdown() {
		socket_.shutdown(asio::socket_base::shutdown_both);
	}
}