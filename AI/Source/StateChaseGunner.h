#pragma once

#include "StateHelpers.h"

class StateChaseGunner final{ //Static class
	_6_DELETED(StateChaseGunner)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateChaseGunner.inl"