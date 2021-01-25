#pragma once

#include "IEntityCreationAttribs.hpp"

namespace Obj{
	template<class T, typename Type>
	struct GunnerCreationAttribs final: public IEntityCreationAttribs<T, Type>{
	public:
		GunnerCreationAttribs<T, Type>();
		~GunnerCreationAttribs<T, Type>() = default;
	};
}

#include "GunnerCreationAttribs.inl"