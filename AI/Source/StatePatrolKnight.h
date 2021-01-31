#pragma once

#include "StateHelpers.h"

class StatePatrolKnight final{ //Static class
	_6_DELETED(StatePatrolKnight)
public:
	static Sim* sim;
	static Entity** entityMoving;
	static int gridRows;
	static int gridCols;
	static int* selectedRow;
	static int* selectedCol;
	static int* selectedTargetRow;
	static int* selectedTargetCol;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
private:
	static std::vector<Vector3> myVec;
	static std::vector<bool> visited;
};

#include "StatePatrolKnight.inl"