#include "LogicSystem.h"
#include "HttpConnection.h"		// cpp�ļ�����,��ֹ������

/**
* @brief ע��url��Ӧ�Ļص�
*/
void LogicSystem::RegisterGet(std::string url, HttpHandler handler) {
	get_handler_.insert(std::make_pair(url, handler));
}

LogicSystem::LogicSystem() {
	RegisterGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
		beast::ostream(connection->response_.body()) << "receive get_test request";
		});
}

/**
* @brief ִ�лص�
*/
bool LogicSystem::HandleGet(std::string url, std::shared_ptr<HttpConnection> connection) {
	if (get_handler_.find(url) == get_handler_.end()) {
		return false;
	}

	// ִ�лص�
	get_handler_[url](connection);
	return true;
}