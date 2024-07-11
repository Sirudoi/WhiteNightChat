#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(net::io_context& io_context, unsigned short& port)
	: io_context_(io_context)
	, acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
	, socket_(io_context) {

}

void CServer::Start() {
	// ��ȡself, ��ֹ�����ص�ʱthis�����Ѿ����ٿ�����ɵ�����, �����ӳ��������������
	// C++�������⣺
	// �˴�Ҳ�������´���һ��shared_ptr, ���ܴ�������������shared_ptr����ͬһ���ڴ�
	// ĳ���ڴ���shared_ptr�����, ����ָ����ڴ��shared_ptrֻ�ܲ��ÿ�������͸�ֵ, ������ջ�����¿�
	auto self = shared_from_this();

	// ��ȡsocket����
	acceptor_.async_accept(socket_, [self](beast::error_code errcode) {
		// ������ĳЩ��������Ҳ�����쳣
		try {
			if (errcode) {
				// ���������������, ���¼�����ȡ
				self->Start();

				return;
			}

			// û�д��򴴽�http���ӹ��������
			// socketû�п�������,Ҫô������Ҫô����ֵ,socket_ʹ�����û������
			// ���HttpConnection�������ӹ�����, acceptor��ȡ���µ����Ӵ˻����socket_��ֵ,�������, �ʴ˴�ʹ��move
			//std::make_shared<HttpConnection>(std::move(socket))->Start();
			std::make_shared<HttpConnection>(std::move(self->socket_))->Start();

		}
		catch (std::exception& e) {
			std::cout << "exception is " << e.what() << std::endl;
			self->Start();
		}
		});
}
