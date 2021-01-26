#pragma once

#include "StateHelpers.h"

class StateIdleGunner final{ //Static class
	_6_DELETED(StateIdleGunner)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateIdleGunner.inl"