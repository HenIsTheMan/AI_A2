#pragma once

#include "StateHelperFuncs.h"

class StateOrcAttack final{ //Static class
	StateOrcAttack() = delete;
	~StateOrcAttack() = delete;
	StateOrcAttack(const StateOrcAttack&) = delete;
	StateOrcAttack(StateOrcAttack&&) noexcept = delete;
	StateOrcAttack& operator=(const StateOrcAttack&) = delete;
	StateOrcAttack& operator=(StateOrcAttack&&) noexcept = delete;
public:
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateOrcAttack.inl"