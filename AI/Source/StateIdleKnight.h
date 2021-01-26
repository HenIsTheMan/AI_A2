#pragma once

#include "StateHelpers.h"

class StateIdleKnight final{ //Static class
	_6_DELETED(StateIdleKnight)
public:
	static bool sm_IsFlatTop;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateIdleKnight.inl"