#pragma once

#include "Vector3.h"

#include "Sim.h"

namespace Obj{
	template <class T>
	static T FindValidLocalPos(const Sim* const sim);

	template <>
	static Vector3 FindValidLocalPos(const Sim* const sim);
}

#include "EntityCreationHelpers.inl"