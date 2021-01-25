#pragma once

#include <vector>

#include "timer.h"

#include "SimRuntimeStatus.hpp"
#include "SimTimeOfDay.hpp"
#include "SimTurn.hpp"

#include "TileType.hpp"

#include "Publisher.h"

class Sim final{
public:
	struct{
		SimRuntimeStatus status = SimRuntimeStatus::Amt;
		float spd = 0.0f;
		float turnDuration = 0.0f;
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

	void GenTileLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key, const float* quickRenderDelay, const bool isFlatTop);
	void RefineTileLayer(const int gridRows, const int gridCols, const unsigned int key, const float* quickRenderDelay);
	void MakeRadialHoleInTileLayer(const int gridRows, const int gridCols, const int row, const int col, const int radius, const float* quickRenderDelay, const bool isFlatTop);

	//* Getters
	const std::vector<TileType>& GetTileLayer() const;
	//*/
private:
	bool isKeySpace;

	int tileWeights[(int)TileType::Amt];

	Publisher* publisher;

	std::vector<TileType> tileLayer;

	StopWatch timer;
};