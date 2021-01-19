#pragma once

#include "StateHelperFuncs.h"

class StateReptileDead final{ //Static class
	StateReptileDead() = delete;
	~StateReptileDead() = delete;
	StateReptileDead(const StateReptileDead&) = delete;
	StateReptileDead(StateReptileDead&&) noexcept = delete;
	StateReptileDead& operator=(const StateReptileDead&) = delete;
	StateReptileDead& operator=(StateReptileDead&&) noexcept = delete;
public:
	static ObjPool<Entity>* im_ObjPool;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateReptileDead.inl"