#include <cassert>

#include "Sim.h"

#include "TileCost.hpp"

Sim::Sim():
	gameSpd(1.0f),
	timeOfDay(TimeOfDay::Amt),
	fogWeights(),
	tileWeights(),
	fogLayer(),
	tileLayer()
{
}

void Sim::ChangeFogWeight(const int index, const int fogWeight){
	fogWeights[index] = fogWeight;
}

void Sim::ChangeTileWeight(const int index, const int tileWeight){
	tileWeights[index] = tileWeight;
}

float& Sim::RetrieveGameSpd(){
	return gameSpd;
}

TimeOfDay& Sim::RetrieveTimeOfDay(){
	return timeOfDay;
}

std::vector<FogType>& Sim::RetrieveFogLayer(){
	return fogLayer;
}

std::vector<TileType>& Sim::RetrieveTileLayer(){
	return tileLayer;
}

float Sim::GetGameSpd() const{
	return gameSpd;
}

TimeOfDay Sim::GetTimeOfDay() const{
	return timeOfDay;
}

const std::vector<FogType>& Sim::GetFogLayer() const{
	return fogLayer;
}

const std::vector<TileType>& Sim::GetTileLayer() const{
	return tileLayer;
}

void Sim::SetGameSpd(const float gameSpd){
	this->gameSpd = gameSpd;
}

void Sim::SetTimeOfDay(const TimeOfDay timeOfDay){
	this->timeOfDay = timeOfDay;
}

void Sim::GenFogLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key){
	int fogSumOfWeights = 0;
	for(int i = 0; i < (int)FogType::Amt; ++i){
		fogSumOfWeights += fogWeights[i];
	}
	if(fogSumOfWeights == 0){
		return;
	}
	srand(key);

	const int gridTotalCells = gridRows * gridCols;
	fogLayer.reserve(gridTotalCells);

	for(int i = 0; i < gridTotalCells; ++i){
		int val = rand() % fogSumOfWeights + 1;

		for(int i = 0; i < (int)FogType::Amt; ++i){
			const int fogWeight = fogWeights[i];
			if(val <= fogWeight){
				fogLayer.emplace_back((FogType)i);
				break;
			}
			val -= fogWeight;
		}
	}

	fogLayer[startRow * gridCols + startCol] = FogType::Inexistent;
}

void Sim::GenTileLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key){
	srand(key);
	const int gridTotalCells = gridRows * gridCols;
	std::vector<int> wallIndices;
	std::vector<int> savedIndices;
	std::vector<bool> visited;

	tileLayer.reserve(gridTotalCells);
	wallIndices.reserve(gridTotalCells);
	savedIndices.reserve(gridTotalCells);
	visited.reserve(gridTotalCells);

	for(int i = 0; i < gridTotalCells; ++i){
		tileLayer.emplace_back(TileType::Wall);
		savedIndices.emplace_back(-1);
		visited.emplace_back(false);
	}

	const int startIndex = startRow * gridCols + startCol;
	tileLayer[startIndex] = TileType::Empty;
	visited[startIndex] = true;

	const int upIndex = (startRow + 1) * gridCols + startCol;
	const int downIndex = (startRow - 1) * gridCols + startCol;
	const int leftIndex = startRow * gridCols + (startCol - 1);
	const int rightIndex = startRow * gridCols + (startCol + 1);
	if(startRow < gridRows - 1){
		savedIndices[upIndex] = startIndex;
		wallIndices.emplace_back(upIndex);
	}
	if(startRow > 0){
		savedIndices[downIndex] = startIndex;
		wallIndices.emplace_back(downIndex);
	}
	if(startCol > 0){
		savedIndices[leftIndex] = startIndex;
		wallIndices.emplace_back(leftIndex);
	}
	if(startCol < gridCols - 1){
		savedIndices[rightIndex] = startIndex;
		wallIndices.emplace_back(rightIndex);
	}

	if(startCol & 1){
		const int ULIndex = (startRow + 1) * gridCols + (startCol - 1);
		const int URIndex = (startRow + 1) * gridCols + (startCol + 1);
		if(startCol > 0 && startRow < gridRows - 1){
			savedIndices[ULIndex] = startIndex;
			wallIndices.emplace_back(ULIndex);
		}
		if(startCol < gridCols - 1 && startRow < gridRows - 1){
			savedIndices[URIndex] = startIndex;
			wallIndices.emplace_back(URIndex);
		}
	} else{
		const int DLIndex = (startRow - 1) * gridCols + (startCol - 1);
		const int DRIndex = (startRow - 1) * gridCols + (startCol + 1);
		if(startCol > 0 && startRow > 0){
			savedIndices[DLIndex] = startIndex;
			wallIndices.emplace_back(DLIndex);
		}
		if(startCol < gridCols - 1 && startRow > 0){
			savedIndices[DRIndex] = startIndex;
			wallIndices.emplace_back(DRIndex);
		}
	}

	while(!wallIndices.empty()){
		const size_t wallIndicesSize = wallIndices.size();

		const int randIndex = rand() % wallIndicesSize;
		const int wallIndex = wallIndices[randIndex];
		visited[wallIndex] = true;
		const int savedIndex = savedIndices[wallIndex];
		wallIndices.erase(wallIndices.begin() + randIndex);

		const int savedX = savedIndex % gridCols;
		const int savedY = savedIndex / gridCols;
		const int wallX = wallIndex % gridCols;
		const int wallY = wallIndex / gridCols;
		const int otherX = (wallX << 1) - savedX; //wallX + xDiff
		const int otherY = (wallY << 1) - savedY; //wallY + yDiff
		const int otherIndex = otherY * gridCols + otherX;

		if(otherX >= 0 && otherX <= gridCols - 1 && otherY >= 0 && otherY <= gridRows - 1 && (visited[savedIndex] ^ visited[otherIndex])){
			tileLayer[wallIndex] = TileType::Empty;
			tileLayer[otherIndex] = TileType::Empty;
			visited[otherIndex] = true;

			const int otherUpIndex = (otherY + 1) * gridCols + otherX;
			const int otherDownIndex = (otherY - 1) * gridCols + otherX;
			const int otherLeftIndex = otherY * gridCols + (otherX - 1);
			const int otherRightIndex = otherY * gridCols + (otherX + 1);
			if(otherY < gridRows - 1){
				savedIndices[otherUpIndex] = otherIndex;
				wallIndices.emplace_back(otherUpIndex);
			}
			if(otherY > 0){
				savedIndices[otherDownIndex] = otherIndex;
				wallIndices.emplace_back(otherDownIndex);
			}
			if(otherX > 0){
				savedIndices[otherLeftIndex] = otherIndex;
				wallIndices.emplace_back(otherLeftIndex);
			}
			if(otherX < gridCols - 1){
				savedIndices[otherRightIndex] = otherIndex;
				wallIndices.emplace_back(otherRightIndex);
			}

			if(otherX & 1){
				const int otherULIndex = (otherY + 1) * gridCols + (otherX - 1);
				const int otherURIndex = (otherY + 1) * gridCols + (otherX + 1);
				if(otherX > 0 && otherY < gridRows - 1){
					savedIndices[otherULIndex] = otherIndex;
					wallIndices.emplace_back(otherULIndex);
				}
				if(otherX < gridCols - 1 && otherY < gridRows - 1){
					savedIndices[otherURIndex] = otherIndex;
					wallIndices.emplace_back(otherURIndex);
				}
			} else{
				const int otherDLIndex = (otherY - 1) * gridCols + (otherX - 1);
				const int otherDRIndex = (otherY - 1) * gridCols + (otherX + 1);
				if(otherX > 0 && otherY > 0){
					savedIndices[otherDLIndex] = otherIndex;
					wallIndices.emplace_back(otherDLIndex);
				}
				if(otherX < gridCols - 1 && otherY > 0){
					savedIndices[otherDRIndex] = otherIndex;
					wallIndices.emplace_back(otherDRIndex);
				}
			}
		}
	}
}

void Sim::RefineTileLayer(const int gridRows, const int gridCols, const unsigned int key){
	int tileSumOfWeights = 0;
	for(int i = 0; i < (int)TileType::Amt; ++i){
		tileSumOfWeights += tileWeights[i];
	}
	if(tileSumOfWeights == 0){
		return;
	}
	srand(key);

	const int gridTotalCells = gridRows * gridCols;
	for(int i = 0; i < gridTotalCells; ++i){
		TileType& type = tileLayer[i];

		if(type == TileType::Wall){
			int val = rand() % tileSumOfWeights + 1;

			for(int i = 0; i < (int)TileType::Amt; ++i){
				const int tileWeight = tileWeights[i];
				if(val <= tileWeight){
					type = (TileType)i;
					break;
				}
				val -= tileWeight;
			}
		}
	}
}

void Sim::MakeRadialHoleInTileLayer(const int gridRows, const int gridCols, const int row, const int col, const int radius){
	std::vector<std::pair<int, int>> myVec;
	myVec.emplace_back(std::make_pair(0, row * gridCols + col));

	while(!myVec.empty()){
		const std::pair<int, int> myPair = myVec.front();
		if(tileLayer[myPair.second] == TileType::Invalid){
			myVec.erase(myVec.begin());
			continue;
		}
		tileLayer[myPair.second] = TileType::Invalid;

		if(myPair.first != radius){
			const int myCol = myPair.second % gridCols;
			const int myRow = myPair.second / gridCols;

			const int upIndex = (myRow + 1) * gridCols + myCol;
			const int downIndex = (myRow - 1) * gridCols + myCol;
			const int leftIndex = myRow * gridCols + (myCol - 1);
			const int rightIndex = myRow * gridCols + (myCol + 1);

			if(myRow < gridRows - 1){
				myVec.emplace_back(std::make_pair(myPair.first + 1, upIndex));
			}
			if(myRow > 0){
				myVec.emplace_back(std::make_pair(myPair.first + 1, downIndex));
			}
			if(myCol > 0){
				myVec.emplace_back(std::make_pair(myPair.first + 1, leftIndex));
			}
			if(myCol < gridCols - 1){
				myVec.emplace_back(std::make_pair(myPair.first + 1, rightIndex));
			}

			if(myCol & 1){
				const int ULIndex = (myRow + 1) * gridCols + (myCol - 1);
				const int URIndex = (myRow + 1) * gridCols + (myCol + 1);
				if(myCol > 0 && myRow < gridRows - 1){
					myVec.emplace_back(std::make_pair(myPair.first + 1, ULIndex));
				}
				if(myCol < gridCols - 1 && myRow < gridRows - 1){
					myVec.emplace_back(std::make_pair(myPair.first + 1, URIndex));
				}
			} else{
				const int DLIndex = (myRow - 1) * gridCols + (myCol - 1);
				const int DRIndex = (myRow - 1) * gridCols + (myCol + 1);
				if(myCol > 0 && myRow > 0){
					myVec.emplace_back(std::make_pair(myPair.first + 1, DLIndex));
				}
				if(myCol < gridCols - 1 && myRow > 0){
					myVec.emplace_back(std::make_pair(myPair.first + 1, DRIndex));
				}
			}
		}

		myVec.erase(myVec.begin());
	}
}