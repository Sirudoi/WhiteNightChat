#pragma once
#include "common.h"
/**
* @brief CServer�ṩ��ȡsocket���ӵĹ��� 
* 
* CServer�����ϴ�socket��ȡ����, ������http�����ദ��
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