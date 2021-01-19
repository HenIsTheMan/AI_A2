#pragma once

#include "StateHelperFuncs.h"

class StateSkeleChase final{ //Static class
	StateSkeleChase() = delete;
	~StateSkeleChase() = delete;
	StateSkeleChase(const StateSkeleChase&) = delete;
	StateSkeleChase(StateSkeleChase&&) noexcept = delete;
	StateSkeleChase& operator=(const StateSkeleChase&) = delete;
	StateSkeleChase& operator=(StateSkeleChase&&) noexcept = delete;
public:
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateSkeleChase.inl"