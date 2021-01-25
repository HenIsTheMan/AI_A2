#pragma once

#include "Vector3.h"

#include "Entity.h"
#include "Sim.h"
#include "TileCost.hpp"

using Entity = Obj::Entity<Vector3, float>; //Can because Entity<Vector3, float> is in another namespace

namespace Obj{
	inline static EntityFacingDir ChooseRandFacingDir();

	template <class T>
	static T FindValidLocalPos(const int gridCols, const Sim* const sim);

	template <>
	static Vector3 FindValidLocalPos(const int gridCols, const Sim* const sim);
}

#include "EntityCreationHelpers.inl"