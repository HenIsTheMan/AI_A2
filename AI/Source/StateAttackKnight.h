#pragma once

#include "StateHelpers.h"

class StateAttackKnight final{ //Static class
	_6_DELETED(StateAttackKnight)
public:
	static bool isDeducted;
	static Sim* sim;
	static int gridCols;
	static int* selectedRow;
	static int* selectedCol;
	static int* selectedTargetRow;
	static int* selectedTargetCol;
	static int* creditsPlayer;
	static int* creditsAI;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateAttackKnight.inl"