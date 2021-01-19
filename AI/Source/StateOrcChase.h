#pragma once

#include "StateHelperFuncs.h"

class StateOrcChase final{ //Static class
	StateOrcChase() = delete;
	~StateOrcChase() = delete;
	StateOrcChase(const StateOrcChase&) = delete;
	StateOrcChase(StateOrcChase&&) noexcept = delete;
	StateOrcChase& operator=(const StateOrcChase&) = delete;
	StateOrcChase& operator=(StateOrcChase&&) noexcept = delete;
public:
	static Grid<float>* im_Grid;
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateOrcChase.inl"