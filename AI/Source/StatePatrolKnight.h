#pragma once

#include "StateHelpers.h"

class StatePatrolKnight final{ //Static class
	_6_DELETED(StatePatrolKnight)
public:
	static Sim* sim;
	static std::vector<Vector3> myVec;
	static std::vector<bool> visited;
	static int gridRows;
	static int gridCols;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StatePatrolKnight.inl"