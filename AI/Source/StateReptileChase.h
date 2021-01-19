#pragma once

#include "StateHelperFuncs.h"

class StateReptileChase final{ //Static class
	StateReptileChase() = delete;
	~StateReptileChase() = delete;
	StateReptileChase(const StateReptileChase&) = delete;
	StateReptileChase(StateReptileChase&&) noexcept = delete;
	StateReptileChase& operator=(const StateReptileChase&) = delete;
	StateReptileChase& operator=(StateReptileChase&&) noexcept = delete;
public:
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateReptileChase.inl"