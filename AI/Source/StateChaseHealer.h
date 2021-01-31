#pragma once

#include "StateHelpers.h"

class StateChaseHealer final{ //Static class
	_6_DELETED(StateChaseHealer)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateChaseHealer.inl"