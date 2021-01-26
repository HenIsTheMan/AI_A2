#pragma once

#include "StateHelpers.h"

class StateWanderHealer final{ //Static class
	_6_DELETED(StateWanderHealer)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateWanderHealer.inl"