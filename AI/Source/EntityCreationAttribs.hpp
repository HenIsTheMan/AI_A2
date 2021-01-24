#pragma once

#include "Vector3.h"

namespace Obj{
	template<class T, typename Type>
	struct EntityCreationAttribs final{ //Passive DS (PDS)/Plain Old DS (PODS)/Plain Old Data (POD)
		EntityCreationAttribs<T, Type>() = default;
		~EntityCreationAttribs<T, Type>() = default;

		EntityTeam im_Team = EntityTeam::None;
		EntityFacingDir im_FacingDir = EntityFacingDir::Invalid;
		int im_Lvl = 1;

		T im_LocalPos = T();

		Type im_Health = Type();
	};
}