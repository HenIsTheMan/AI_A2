#pragma once

#include "StateShared.h"

class StateIdleHealer final{ //Static class
	_6_DELETED(StateIdleHealer)
public:
	static bool sm_IsFlatTop;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateIdleHealer.inl"