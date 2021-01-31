#pragma once

#include "StateShared.h"

class StatePatrolGunner final{ //Static class
	_6_DELETED(StatePatrolGunner)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StatePatrolGunner.inl"