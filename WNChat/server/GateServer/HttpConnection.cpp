#include "HttpConnection.h"
#include "LogicSystem.h"
HttpConnection::HttpConnection(tcp::socket socket)
	: socket_(std::move(socket)) {
}

/**
* @brief ����http����
*/
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

/**
* @brief ��������
*/
void HttpConnection::HandleRequest() {
	// ��ȡ�汾
	response_.version(request_.version());
	response_.keep_alive(false);
	if (request_.method() == http::verb::get) {
		// ����ע���Ӧ·��
		bool res = LogicSystem::GetInstance()->HandleGet(request_.target(), shared_from_this());
		//bool res = true;
		// ·��ʧ��
		if (res) {
			response_.result(http::status::not_found);
			response_.set(http::field::content_type, "text/plain");
			beast::ostream(response_.body()) << "url not found\n";
			// �ͻ���Ӧ��
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
* @brief Ӧ��ͻ���
*/
void HttpConnection::WriteResponse() {
	auto self = shared_from_this();
	response_.content_length(response_.body().size());
	http::async_write(socket_, response_, [self](beast::error_code errcode, std::size_t bytes_transferred) {
		// �رշ��Ͷ�
		self->socket_.shutdown(tcp::socket::shutdown_send, errcode);
		// ȡ����ʱ��
		self->deadline_.cancel();
		});
}

/**
* @brief ��ⶨʱ��
* 
* WriteResponse�Ļص�һֱ������������ʱ������
* WriteResponse�������رն�ʱ��
* TODO:��������time_wait״̬, ��Ҫ���һ��
*/
void HttpConnection::CheckDeadline() {
	auto self = shared_from_this();
	deadline_.async_wait([self](beast::error_code errcode) {
		// ������������Ԥ��ʱ����û�л�Ӧ
		if (!errcode) {
			self->socket_.close(errcode);
		}
		});
}