#pragma once

#include "StateHelpers.h"

class StateChaseKnight final{ //Static class
	_6_DELETED(StateChaseKnight)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateChaseKnight.inl"