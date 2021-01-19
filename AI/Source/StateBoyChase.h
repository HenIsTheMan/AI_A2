#pragma once

#include "StateHelperFuncs.h"

class StateBoyChase final{ //Static class
	StateBoyChase() = delete;
	~StateBoyChase() = delete;
	StateBoyChase(const StateBoyChase&) = delete;
	StateBoyChase(StateBoyChase&&) noexcept = delete;
	StateBoyChase& operator=(const StateBoyChase&) = delete;
	StateBoyChase& operator=(StateBoyChase&&) noexcept = delete;
public:
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateBoyChase.inl"