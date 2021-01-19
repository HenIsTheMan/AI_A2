#pragma once

#include "StateHelperFuncs.h"

class StateSkeleIdle final{ //Static class
	StateSkeleIdle() = delete;
	~StateSkeleIdle() = delete;
	StateSkeleIdle(const StateSkeleIdle&) = delete;
	StateSkeleIdle(StateSkeleIdle&&) noexcept = delete;
	StateSkeleIdle& operator=(const StateSkeleIdle&) = delete;
	StateSkeleIdle& operator=(StateSkeleIdle&&) noexcept = delete;
public:
	static float im_ElapsedTime;
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateSkeleIdle.inl"