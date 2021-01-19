#pragma once

#include "StateHelperFuncs.h"

class StateBoyImmune final{ //Static class
	StateBoyImmune() = delete;
	~StateBoyImmune() = delete;
	StateBoyImmune(const StateBoyImmune&) = delete;
	StateBoyImmune(StateBoyImmune&&) noexcept = delete;
	StateBoyImmune& operator=(const StateBoyImmune&) = delete;
	StateBoyImmune& operator=(StateBoyImmune&&) noexcept = delete;
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateBoyImmune.inl"