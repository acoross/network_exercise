//
//  Log.hpp
//  TcpNetwork
//
//  Created by 신일환 on 2016. 8. 19..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef Log_hpp
#define Log_hpp

#include <stdio.h>
#include <utility>

namespace util {
	class Logger {
	public:
		template <typename... Args>
		static void Log(const char* const format, Args&&... args) {
			printf(format, std::forward<Args>(args)...);
		}

		static void Log(const char* const format) {
			printf(format);
		}
	};
}

#endif /* Log_hpp */
