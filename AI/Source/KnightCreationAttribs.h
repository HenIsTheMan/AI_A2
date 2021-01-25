#pragma once

#include "EntityCreationAttribs.hpp"

namespace Obj{
	template<class T, typename Type>
	struct KnightCreationAttribs final: public EntityCreationAttribs<T, Type>{
	public:
		KnightCreationAttribs<T, Type>();
		~KnightCreationAttribs<T, Type>() = default;
	};
}

#include "KnightCreationAttribs.inl"