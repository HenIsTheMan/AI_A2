#pragma once

#include "StateHelperFuncs.h"

class StateOrcIdle final{ //Static class
	StateOrcIdle() = delete;
	~StateOrcIdle() = delete;
	StateOrcIdle(const StateOrcIdle&) = delete;
	StateOrcIdle(StateOrcIdle&&) noexcept = delete;
	StateOrcIdle& operator=(const StateOrcIdle&) = delete;
	StateOrcIdle& operator=(StateOrcIdle&&) noexcept = delete;
public:
	static float im_ElapsedTime;
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateOrcIdle.inl"