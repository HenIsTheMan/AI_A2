#pragma once

#include "StateHelperFuncs.h"

class StateOrcPatrol final{ //Static class
	StateOrcPatrol() = delete;
	~StateOrcPatrol() = delete;
	StateOrcPatrol(const StateOrcPatrol&) = delete;
	StateOrcPatrol(StateOrcPatrol&&) noexcept = delete;
	StateOrcPatrol& operator=(const StateOrcPatrol&) = delete;
	StateOrcPatrol& operator=(StateOrcPatrol&&) noexcept = delete;
public:
	static float im_ElapsedTime;
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateOrcPatrol.inl"