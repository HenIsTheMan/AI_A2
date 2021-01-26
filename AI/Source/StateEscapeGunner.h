#pragma once

#include "StateHelpers.h"

class StateEscapeGunner final{ //Static class
	_6_DELETED(StateEscapeGunner)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateEscapeGunner.inl"