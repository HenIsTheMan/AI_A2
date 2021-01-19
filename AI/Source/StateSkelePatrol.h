#pragma once

#include "StateHelperFuncs.h"

class StateSkelePatrol final{ //Static class
	StateSkelePatrol() = delete;
	~StateSkelePatrol() = delete;
	StateSkelePatrol(const StateSkelePatrol&) = delete;
	StateSkelePatrol(StateSkelePatrol&&) noexcept = delete;
	StateSkelePatrol& operator=(const StateSkelePatrol&) = delete;
	StateSkelePatrol& operator=(StateSkelePatrol&&) noexcept = delete;
public:
	static float im_ElapsedTime;
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateSkelePatrol.inl"