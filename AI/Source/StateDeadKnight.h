#pragma once

#include "StateHelpers.h"

class StateDeadKnight final{ //Static class
	_6_DELETED(StateDeadKnight)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateDeadKnight.inl"