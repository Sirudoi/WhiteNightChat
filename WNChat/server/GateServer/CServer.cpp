#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(net::io_context& io_context, unsigned short& port)
	: io_context_(io_context)
	, acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
	, socket_(io_context) {

}

void CServer::Start() {
	// 获取self, 防止触发回调时this对象已经销毁可能造成的问题, 用于延长该类的生命周期
	// C++经典问题：
	// 此处也不可以新创建一个shared_ptr, 不能创造两个独立的shared_ptr管理同一块内存
	// 某块内存有shared_ptr管理后, 后续指向该内存的shared_ptr只能采用拷贝构造和赋值, 不能在栈上重新开
	auto self = shared_from_this();

	// 获取socket连接
	acceptor_.async_accept(socket_, [self](beast::error_code errcode) {
		// 网络中某些正常连接也会抛异常
		try {
			if (errcode) {
				// 出错则放弃此连接, 重新监听获取
				self->Start();

				return;
			}

			// 没有错则创建http连接管理该连接
			// socket没有拷贝构造,要么传引用要么传右值,socket_使用引用会出问题
			// 如果HttpConnection处理连接过程中, acceptor获取了新的连接此会更改socket_的值,会出问题, 故此处使用move
			//std::make_shared<HttpConnection>(std::move(socket))->Start();
			std::make_shared<HttpConnection>(std::move(self->socket_))->Start();

		}
		catch (std::exception& e) {
			std::cout << "exception is " << e.what() << std::endl;
			self->Start();
		}
		});
}
