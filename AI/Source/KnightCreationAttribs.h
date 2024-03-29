#pragma once

#include "IEntityCreationAttribs.hpp"

#include "EntityCreationHelpers.h"

namespace Obj{
	template <class T, typename Type>
	struct KnightCreationAttribs final: public IEntityCreationAttribs<T, Type>{
	public:
		KnightCreationAttribs<T, Type>() = delete;
		KnightCreationAttribs<T, Type>(const int gridCols, const Sim* const sim, const bool isFlatTop);
		~KnightCreationAttribs<T, Type>() = default;
	};
}

#include "KnightCreationAttribs.inl"