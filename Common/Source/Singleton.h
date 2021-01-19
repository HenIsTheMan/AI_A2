#pragma once

#ifndef _4_DELETED
#define _4_DELETED(...) \
		__VA_ARGS__(const __VA_ARGS__&) = delete; \
		__VA_ARGS__(__VA_ARGS__&&) noexcept = delete; \
		__VA_ARGS__& operator=(const __VA_ARGS__&) = delete; \
		__VA_ARGS__& operator=(__VA_ARGS__&&) noexcept = delete;
#endif

template <class T>
class Singleton{ //Abstract class
	_4_DELETED(Singleton<T>)
public:
	~Singleton<T>() = default; //Not virtual as polymorphism is impossible here

	static T* const RetrieveGlobalObjPtr();
	static void Destroy();
protected:
	Singleton<T>() = default;
private:
	static T* objPtr;
};

#include "Singleton.inl"