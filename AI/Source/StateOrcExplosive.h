#pragma once

#include "StateHelperFuncs.h"

class StateOrcExplosive final{ //Static class
	StateOrcExplosive() = delete;
	~StateOrcExplosive() = delete;
	StateOrcExplosive(const StateOrcExplosive&) = delete;
	StateOrcExplosive(StateOrcExplosive&&) noexcept = delete;
	StateOrcExplosive& operator=(const StateOrcExplosive&) = delete;
	StateOrcExplosive& operator=(StateOrcExplosive&&) noexcept = delete;
public:
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateOrcExplosive.inl"