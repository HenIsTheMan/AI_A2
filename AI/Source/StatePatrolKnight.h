#pragma once

#include "StateHelpers.h"

class StatePatrolKnight final{ //Static class
	_6_DELETED(StatePatrolKnight)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StatePatrolKnight.inl"