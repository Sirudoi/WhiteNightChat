#pragma once
#include <memory>
template<typename T>
class Singleton {
protected:
	Singleton() = default;
	Singleton(const Singleton<T>&) = delete;
	Singleton& operator=(const Singleton<T>&) = delete;
	static std::shared_ptr<T> instance_;

public:
	static std::shared_ptr<T> GetInstance() {
		// 使用std::once_flag, 保证只初始化一次, 来控制单例
		static std::once_flag s_flag;
		std::call_once(s_flag, [&]() {
			// 这里不用make_shared, 子类基础之后构造函数会私有, make_shared无法处理这种情况
			instance_ = std::shared_ptr<T>(new T);
			});

		return instance_;
	}

	~Singleton() {
		// TODO:LOG
		std::cout << "singleton is destruct" << std::endl;
	}
};

template<typename T>
std::shared_ptr<T> Singleton<T>::instance_ = nullptr;