#pragma once

#include "StateShared.h"

#include "Publisher.h"

class StateDeadHealer final{ //Static class
	_6_DELETED(StateDeadHealer)
public:
	static Publisher* sm_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateDeadHealer.inl"