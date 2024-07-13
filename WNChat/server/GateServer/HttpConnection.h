#pragma once
#include "common.h"

class LogicSystem;
/**
* @brief HttpConnection 提供管理连接的功能
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
	beast::flat_buffer buffer_{ 8192 };						// 读写缓冲区,初始化为8k大小
	http::request<http::dynamic_body> request_;
	http::response<http::dynamic_body> response_;
	net::steady_timer deadline_{ 
		socket_.get_executor(), std::chrono::seconds(60) };	// 定时器,设定60秒判断请求是否超时
};