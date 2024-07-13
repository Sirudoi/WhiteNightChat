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
		// ʹ��std::once_flag, ��ֻ֤��ʼ��һ��, �����Ƶ���
		static std::once_flag s_flag;
		std::call_once(s_flag, [&]() {
			// ���ﲻ��make_shared, �������֮���캯����˽��, make_shared�޷������������
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