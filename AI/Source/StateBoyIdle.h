#pragma once

#include "StateHelperFuncs.h"

class StateBoyIdle final{ //Static class
	StateBoyIdle() = delete;
	~StateBoyIdle() = delete;
	StateBoyIdle(const StateBoyIdle&) = delete;
	StateBoyIdle(StateBoyIdle&&) noexcept = delete;
	StateBoyIdle& operator=(const StateBoyIdle&) = delete;
	StateBoyIdle& operator=(StateBoyIdle&&) noexcept = delete;
public:
	static float im_ElapsedTime;
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateBoyIdle.inl"