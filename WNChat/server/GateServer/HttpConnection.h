#pragma once
#include "common.h"

class LogicSystem;
/**
* @brief HttpConnection �ṩ�������ӵĹ���
*/
class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
	friend class LogicSystem;

	HttpConnection(tcp::socket socket);
	void Start();

private:
	void CheckDeadline();
	void WriteResponse();
	void HandleRequest();

	tcp::socket socket_;
	beast::flat_buffer buffer_{ 8192 };						// ��д������,��ʼ��Ϊ8k��С
	http::request<http::dynamic_body> request_;
	http::response<http::dynamic_body> response_;
	net::steady_timer deadline_{ 
		socket_.get_executor(), std::chrono::seconds(60) };	// ��ʱ��,�趨60���ж������Ƿ�ʱ
};