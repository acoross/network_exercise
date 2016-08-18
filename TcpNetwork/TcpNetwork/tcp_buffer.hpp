//
//  tcp_buffer.hpp
//  TcpNetwork
//
//  Created by 신일환 on 2016. 8. 18..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef tcp_buffer_hpp
#define tcp_buffer_hpp

#include <stdio.h>
#include <stdlib.h>
#include <memory>

namespace network {

	class TcpBuffer {
	public:
		bool has_space(int size) {
			return (space() >= size);
		}

		void write(char* data, int size) {
			if (has_space(size)) {
				::memcpy(empty_buffer(), data, size);
			}
		}

		char* raw() {
			return data_;
		}

		constexpr int data_size() const {
			return SIZ;
		}

		char* empty_buffer() {
			return data_ + data_size_;
		}

		int space() const {
			return SIZ - data_size_;
		}

	private:
		enum {
			SIZ = 1024
		};
		char data_[SIZ]{0,};

		int data_size_{0};
	};
	using TcpBufferSP = std::shared_ptr<TcpBuffer>;
}

#endif /* tcp_buffer_hpp */
