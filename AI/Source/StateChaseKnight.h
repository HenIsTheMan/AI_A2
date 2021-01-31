#pragma once

#include "StateHelpers.h"

class StateChaseKnight final{ //Static class
	_6_DELETED(StateChaseKnight)
public:
	static Sim* sim;
	static Entity** entityMoving;
	static AStar<Vector3, float>* myAStar;
	static std::vector<Vector3>* myShortestPath;
	static HexGrid<float>::GridType gridType;
	static int gridRows;
	static int gridCols;
	static int* selectedRow;
	static int* selectedCol;
	static int* selectedTargetRow;
	static int* selectedTargetCol;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateChaseKnight.inl"