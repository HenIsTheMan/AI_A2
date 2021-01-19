#pragma once

#include "StateHelperFuncs.h"

class StateOrcDead final{ //Static class
	StateOrcDead() = delete;
	~StateOrcDead() = delete;
	StateOrcDead(const StateOrcDead&) = delete;
	StateOrcDead(StateOrcDead&&) noexcept = delete;
	StateOrcDead& operator=(const StateOrcDead&) = delete;
	StateOrcDead& operator=(StateOrcDead&&) noexcept = delete;
public:
	static ObjPool<Entity>* im_ObjPool;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateOrcDead.inl"