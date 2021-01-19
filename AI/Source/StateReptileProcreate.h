#pragma once

#include "StateHelperFuncs.h"

class StateReptileProcreate final{ //Static class
	StateReptileProcreate() = delete;
	~StateReptileProcreate() = delete;
	StateReptileProcreate(const StateReptileProcreate&) = delete;
	StateReptileProcreate(StateReptileProcreate&&) noexcept = delete;
	StateReptileProcreate& operator=(const StateReptileProcreate&) = delete;
	StateReptileProcreate& operator=(StateReptileProcreate&&) noexcept = delete;
public:
	static Publisher* im_Publisher;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateReptileProcreate.inl"