#pragma once

#include "StateHelpers.h"

class StateFollowHealer final{ //Static class
	_6_DELETED(StateFollowHealer)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateFollowHealer.inl"