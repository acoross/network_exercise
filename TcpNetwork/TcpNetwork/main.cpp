//
//  main.cpp
//  TcpNetwork
//
//  Created by 신일환 on 2016. 8. 18..
//  Copyright © 2016년 acoross. All rights reserved.
//

#include <iostream>

#include "tcp_client.hpp"

int main(int argc, const char * argv[]) {
	// insert code here...

	asio::io_service ios;
	network::TcpClient client(ios);
	client.Connect("localhost", "12121");

	std::cout << "Hello, World!\n";

	ios.run();
    return 0;
}
