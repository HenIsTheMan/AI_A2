#pragma once

#include "StateHelperFuncs.h"

class StateReptilePatrol final{ //Static class
	StateReptilePatrol() = delete;
	~StateReptilePatrol() = delete;
	StateReptilePatrol(const StateReptilePatrol&) = delete;
	StateReptilePatrol(StateReptilePatrol&&) noexcept = delete;
	StateReptilePatrol& operator=(const StateReptilePatrol&) = delete;
	StateReptilePatrol& operator=(StateReptilePatrol&&) noexcept = delete;
public:
	static float im_ElapsedTime;
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;
	static Vector3 im_CommonDirs[2];

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateReptilePatrol.inl"