#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "CServer.h"
const unsigned short g_port = 8080;	// 端口
const size_t g_ioc_thd = 1;			// ioc线程个数

int main()
{
	try {
		net::io_context ioc { g_ioc_thd };
		// 信号集:捕获SIGINT SIGTERM
		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
		signals.async_wait([&ioc](const boost::system::error_code& errcode, int signal_number) {
			if (errcode) {
				return;
			}
			ioc.stop();
			});

		std::make_shared<CServer>(ioc, g_port)->Start();
		ioc.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}