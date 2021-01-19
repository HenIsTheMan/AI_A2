#pragma once

#include "StateHelperFuncs.h"

class StateBoyPatrol final{ //Static class
	StateBoyPatrol() = delete;
	~StateBoyPatrol() = delete;
	StateBoyPatrol(const StateBoyPatrol&) = delete;
	StateBoyPatrol(StateBoyPatrol&&) noexcept = delete;
	StateBoyPatrol& operator=(const StateBoyPatrol&) = delete;
	StateBoyPatrol& operator=(StateBoyPatrol&&) noexcept = delete;
public:
	static float im_ElapsedTime;
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateBoyPatrol.inl"