#pragma once

#include "StateHelperFuncs.h"

class StateReptileIdle final{ //Static class
	StateReptileIdle() = delete;
	~StateReptileIdle() = delete;
	StateReptileIdle(const StateReptileIdle&) = delete;
	StateReptileIdle(StateReptileIdle&&) noexcept = delete;
	StateReptileIdle& operator=(const StateReptileIdle&) = delete;
	StateReptileIdle& operator=(StateReptileIdle&&) noexcept = delete;
public:
	static float im_ElapsedTime;
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateReptileIdle.inl"