#pragma once

#include "StateHelperFuncs.h"

class StateSkeleDeadButNotDead final{ //Static class
	StateSkeleDeadButNotDead() = delete;
	~StateSkeleDeadButNotDead() = delete;
	StateSkeleDeadButNotDead(const StateSkeleDeadButNotDead&) = delete;
	StateSkeleDeadButNotDead(StateSkeleDeadButNotDead&&) noexcept = delete;
	StateSkeleDeadButNotDead& operator=(const StateSkeleDeadButNotDead&) = delete;
	StateSkeleDeadButNotDead& operator=(StateSkeleDeadButNotDead&&) noexcept = delete;
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateSkeleDeadButNotDead.inl"