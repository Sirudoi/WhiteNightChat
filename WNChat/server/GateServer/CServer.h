#pragma once
#include "common.h"
/**
* @brief CServer提供读取socket连接的功能 
* 
* CServer将不断从socket获取连接, 并创建http连接类处理
*/

class CServer : public std::enable_shared_from_this<CServer> {
public:
	CServer(net::io_context& io_context, const unsigned short& port);
	void Start();

private:
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	net::io_context& io_context_;
};