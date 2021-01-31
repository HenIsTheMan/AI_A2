#pragma once

#include "StateShared.h"

#include "Publisher.h"

class StateDeadGunner final{ //Static class
	_6_DELETED(StateDeadGunner)
public:
	static Publisher* sm_Publisher;
	static Entity** entityMoving;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateDeadGunner.inl"