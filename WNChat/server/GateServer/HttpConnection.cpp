#include "HttpConnection.h"

HttpConnection::HttpConnection(tcp::socket socket)
	: socket_(std::move(socket)) {
}

void HttpConnection::Start() {
	auto self = shared_from_this();
	// beast异步读取, 用来读取请求头
	http::async_read(socket_, buffer_, request_ ,[self](beast::error_code errcode, std::size_t bytes_transferred) {
		try {
			if (errcode) {
				std::cout << "http error, code:" << errcode.what() << std::endl;
				return;
			}

			// 不处理粘包, 忽略bytes_transferred
			boost::ignore_unused(bytes_transferred);
			// 处理请求
			self->HandleRequest();
			self->CheckDeadline();
		}
		catch (const std::exception& e) {
			std::cout << "exception is " << e.what() << std::endl;
		}
		});

}

void HttpConnection::HandleRequest() {
	// 获取版本
	response_.version(request_.version());
	response_.keep_alive(false);
	if (request_.method() == http::verb::get) {

	}
}