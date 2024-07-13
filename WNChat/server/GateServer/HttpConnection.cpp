#include "HttpConnection.h"
#include "LogicSystem.h"
HttpConnection::HttpConnection(tcp::socket socket)
	: socket_(std::move(socket)) {
}

/**
* @brief 建立http连接
*/
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

/**
* @brief 处理请求
*/
void HttpConnection::HandleRequest() {
	// 获取版本
	response_.version(request_.version());
	response_.keep_alive(false);
	if (request_.method() == http::verb::get) {
		// 交由注册对应路由
		bool res = LogicSystem::GetInstance()->HandleGet(request_.target(), shared_from_this());
		//bool res = true;
		// 路由失败
		if (res) {
			response_.result(http::status::not_found);
			response_.set(http::field::content_type, "text/plain");
			beast::ostream(response_.body()) << "url not found\n";
			// 客户端应答
			WriteResponse();
			return;
		}
		response_.result(http::status::ok);
		response_.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}
}

/**
* @brief 应答客户端
*/
void HttpConnection::WriteResponse() {
	auto self = shared_from_this();
	response_.content_length(response_.body().size());
	http::async_write(socket_, response_, [self](beast::error_code errcode, std::size_t bytes_transferred) {
		// 关闭发送端
		self->socket_.shutdown(tcp::socket::shutdown_send, errcode);
		// 取消定时器
		self->deadline_.cancel();
		});
}

/**
* @brief 检测定时器
* 
* WriteResponse的回调一直不触发会引起定时器触发
* WriteResponse触发后会关闭定时器
* TODO:这里会出现time_wait状态, 需要解决一下
*/
void HttpConnection::CheckDeadline() {
	auto self = shared_from_this();
	deadline_.async_wait([self](beast::error_code errcode) {
		// 连接正常但是预定时间内没有回应
		if (!errcode) {
			self->socket_.close(errcode);
		}
		});
}