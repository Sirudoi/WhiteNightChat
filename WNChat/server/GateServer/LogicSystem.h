#pragma once
#include "common.h"
#include "Singleton.h"

// 防止头文件重复包含,这里头文件不包含HttpConnection, 在cpp包含
class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

class LogicSystem : public Singleton<LogicSystem> {
public:
	friend class Singleton<LogicSystem>;

	~LogicSystem() {}
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
	void RegisterGet(std::string, HttpHandler);

private:
	LogicSystem();

	std::unordered_map<std::string, HttpHandler> get_handler_;
};