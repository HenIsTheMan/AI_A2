#pragma once

#include "IEntityCreationAttribs.hpp"

namespace Obj{
	template<class T, typename Type>
	struct KnightCreationAttribs final: public IEntityCreationAttribs<T, Type>{
	public:
		KnightCreationAttribs<T, Type>();
		~KnightCreationAttribs<T, Type>() = default;
	};
}

#include "KnightCreationAttribs.inl"