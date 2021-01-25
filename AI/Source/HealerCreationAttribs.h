#pragma once

#include "IEntityCreationAttribs.hpp"

namespace Obj{
	template<class T, typename Type>
	struct HealerCreationAttribs final: public IEntityCreationAttribs<T, Type>{
	public:
		HealerCreationAttribs<T, Type>();
		~HealerCreationAttribs<T, Type>() = default;
	};
}

#include "HealerCreationAttribs.inl"