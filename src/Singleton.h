#pragma once

#include <mutex>

template <typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (_instance == nullptr)
		{
			_instance = new T();
		}
		return _instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static T* _instance;
	static std::mutex _mutex;

};

// Init static members
template <typename T>
T* Singleton<T>::_instance = nullptr;

template <typename T>
std::mutex Singleton<T>::_mutex;