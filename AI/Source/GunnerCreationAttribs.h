#pragma once

#include "IEntityCreationAttribs.hpp"

#include "EntityCreationHelpers.h"

namespace Obj{
	template <class T, typename Type>
	struct GunnerCreationAttribs final: public IEntityCreationAttribs<T, Type>{
	public:
		GunnerCreationAttribs<T, Type>() = delete;
		GunnerCreationAttribs<T, Type>(const int gridCols, const Sim* const sim, const bool isFlatTop);
		~GunnerCreationAttribs<T, Type>() = default;
	};
}

#include "GunnerCreationAttribs.inl"