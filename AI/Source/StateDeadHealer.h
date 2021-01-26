#pragma once

#include "StateHelpers.h"

class StateDeadHealer final{ //Static class
	_6_DELETED(StateDeadHealer)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateDeadHealer.inl"