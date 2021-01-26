#pragma once

#ifndef _6_DELETED
#define _6_DELETED(...) \
		__VA_ARGS__() = delete; \
		\
		__VA_ARGS__(const __VA_ARGS__&) = delete; \
		__VA_ARGS__(__VA_ARGS__&&) noexcept = delete; \
		__VA_ARGS__& operator=(const __VA_ARGS__&) = delete; \
		__VA_ARGS__& operator=(__VA_ARGS__&&) noexcept = delete; \
		\
		~__VA_ARGS__() = delete;
#endif

#include "Entity.h"

using Entity = Obj::Entity<Vector3, float>;