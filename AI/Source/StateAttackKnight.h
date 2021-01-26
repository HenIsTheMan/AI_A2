#pragma once

#include "StateHelpers.h"

class StateAttackKnight final{ //Static class
	_6_DELETED(StateAttackKnight)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateAttackKnight.inl"