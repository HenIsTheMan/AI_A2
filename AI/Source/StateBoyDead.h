#pragma once

#include "StateHelperFuncs.h"

class StateBoyDead final{ //Static class
	StateBoyDead() = delete;
	~StateBoyDead() = delete;
	StateBoyDead(const StateBoyDead&) = delete;
	StateBoyDead(StateBoyDead&&) noexcept = delete;
	StateBoyDead& operator=(const StateBoyDead&) = delete;
	StateBoyDead& operator=(StateBoyDead&&) noexcept = delete;
public:
	static ObjPool<Entity>* im_ObjPool;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateBoyDead.inl"