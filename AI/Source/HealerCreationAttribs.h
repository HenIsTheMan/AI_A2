#pragma once

#include "IEntityCreationAttribs.hpp"

#include "EntityCreationHelpers.h"

namespace Obj{
	template <class T, typename Type>
	struct HealerCreationAttribs final: public IEntityCreationAttribs<T, Type>{
	public:
		HealerCreationAttribs<T, Type>() = delete;
		HealerCreationAttribs<T, Type>(const int gridCols, const Sim* const sim);
		~HealerCreationAttribs<T, Type>() = default;
	};
}

#include "HealerCreationAttribs.inl"