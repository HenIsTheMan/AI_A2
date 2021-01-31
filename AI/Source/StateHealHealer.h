#pragma once

#include "StateShared.h"

class StateHealHealer final{ //Static class
	_6_DELETED(StateHealHealer)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateHealHealer.inl"