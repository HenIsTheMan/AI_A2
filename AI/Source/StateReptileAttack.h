#pragma once

#include "StateHelperFuncs.h"

class StateReptileAttack final{ //Static class
	StateReptileAttack() = delete;
	~StateReptileAttack() = delete;
	StateReptileAttack(const StateReptileAttack&) = delete;
	StateReptileAttack(StateReptileAttack&&) noexcept = delete;
	StateReptileAttack& operator=(const StateReptileAttack&) = delete;
	StateReptileAttack& operator=(StateReptileAttack&&) noexcept = delete;
public:
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateReptileAttack.inl"