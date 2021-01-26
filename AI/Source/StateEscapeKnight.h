#pragma once

#include "StateHelpers.h"

class StateEscapeKnight final{ //Static class
	_6_DELETED(StateEscapeKnight)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateEscapeKnight.inl"