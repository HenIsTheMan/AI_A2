#pragma once

#include "Vector3.h"

#include "EntityTeam.hpp"
#include "EntityFacingDir.hpp"

namespace Obj{
	template <class T, typename Type>
	class IEntityCreationAttribs{ //Passive DS (PDS)/Plain Old DS (PODS)/Plain Old Data (POD)
	public:
		virtual ~IEntityCreationAttribs<T, Type>() = default;

		EntityTeam im_Team = EntityTeam::None;
		EntityFacingDir im_FacingDir = EntityFacingDir::Invalid;
		int im_Lvl = 1;

		T im_LocalPos = T();

		Type im_Health = Type();
	protected:
		IEntityCreationAttribs<T, Type>() = default;
	};
}