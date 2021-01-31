#pragma once

#include "StateShared.h"

class StateAttackGunner final{ //Static class
	_6_DELETED(StateAttackGunner)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateAttackGunner.inl"