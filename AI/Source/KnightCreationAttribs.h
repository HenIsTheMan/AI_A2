#pragma once

#include "IEntityCreationAttribs.hpp"

#include "EntityCreationHelpers.h"

namespace Obj{
	template <class T, typename Type>
	struct KnightCreationAttribs final: public IEntityCreationAttribs<T, Type>{
	public:
		KnightCreationAttribs<T, Type>();
		KnightCreationAttribs<T, Type>(const Sim* const sim);
		~KnightCreationAttribs<T, Type>() = default;
	};
}

#include "KnightCreationAttribs.inl"