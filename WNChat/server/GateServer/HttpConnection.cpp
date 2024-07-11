#include "HttpConnection.h"

HttpConnection::HttpConnection(tcp::socket socket)
	: socket_(std::move(socket)) {
}

void HttpConnection::Start() {
	auto self = shared_from_this();
	// beast�첽��ȡ, ������ȡ����ͷ
	http::async_read(socket_, buffer_, request_ ,[self](beast::error_code errcode, std::size_t bytes_transferred) {
		try {
			if (errcode) {
				std::cout << "http error, code:" << errcode.what() << std::endl;
				return;
			}

			// ������ճ��, ����bytes_transferred
			boost::ignore_unused(bytes_transferred);
			// ��������
			self->HandleRequest();
			self->CheckDeadline();
		}
		catch (const std::exception& e) {
			std::cout << "exception is " << e.what() << std::endl;
		}
		});

}

void HttpConnection::HandleRequest() {
	// ��ȡ�汾
	response_.version(request_.version());
	response_.keep_alive(false);
	if (request_.method() == http::verb::get) {

	}
}