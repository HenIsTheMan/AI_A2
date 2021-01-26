#pragma once

#include <vector>

#include "timer.h"

#include "SimRuntimeStatus.hpp"
#include "SimTimeOfDay.hpp"
#include "SimTurn.hpp"

#include "TileType.hpp"

#include "Publisher.h"

#include "Entity.h"

class Sim final{
public:
	struct{
		SimRuntimeStatus status = SimRuntimeStatus::Amt;
		float spd = 0.0f;
		float turnDurationAI = 0.0f;
		float turnDurationEnvironment = 0.0f;
		float turnDurationPlayer = 0.0f;
		float turnElapsedTime = 0.0f;
		SimTurn turn = SimTurn::Amt;
		float timeOfDayDuration = 0.0f;
		float timeOfDayElapsedTime = 0.0f;
		SimTimeOfDay timeOfDay = SimTimeOfDay::Amt;
	};

	Sim();
	~Sim() = default;

	void Update(const double dt);

	void ChangeTileWeight(const int index, const int tileWeight);
	void InitEntityLayer(const int gridRows, const int gridCols);
	void OnEntityActivated(const int gridCols, Entity* const entity);
	void OnEntityDeactivated(const int gridCols, const int row, const int col);

	void GenTileLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key, const float* quickRenderDelay, const bool isFlatTop);
	void RefineTileLayer(const int gridRows, const int gridCols, const unsigned int key, const float* quickRenderDelay);
	void MakeRadialHoleInTileLayer(const int gridRows, const int gridCols, const int row, const int col, const int radius, const float* quickRenderDelay, const bool isFlatTop);

	//* Getters
	const std::vector<Entity*>& GetEntityLayer() const;
	const std::vector<TileType>& GetTileLayer() const;
	//*/
private:
	bool isKeySpace;
	float elapsedTime;

	int tileWeights[(int)TileType::Amt];

	Publisher* publisher;

	std::vector<Entity*> entityLayer;
	std::vector<TileType> tileLayer;

	StopWatch timer;

	void UpdateEnvironment();
	void UpdateEmpty(TileType& type);
	void UpdateSoil(TileType& type);
	void UpdateFire(TileType& type);
	void UpdateWater(TileType& type);
	void UpdateGrass(TileType& type);
	void UpdateMud(TileType& type);
};