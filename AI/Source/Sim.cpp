#include <cassert>

#include "Sim.h"

#include "App.h"

#include "MyMath.h"
#include "TileCost.hpp"

Sim::Sim():
	isKeySpace(false),
	fogWeights(),
	tileWeights(),
	fogLayer(),
	tileLayer(),
	timer()
{
	timer.startTimer();
}

void Sim::ChangeFogWeight(const int index, const int fogWeight){
	fogWeights[index] = fogWeight;
}

void Sim::ChangeTileWeight(const int index, const int tileWeight){
	tileWeights[index] = tileWeight;
}

const std::vector<FogType>& Sim::GetFogLayer() const{
	return fogLayer;
}

const std::vector<TileType>& Sim::GetTileLayer() const{
	return tileLayer;
}

void Sim::GenFogLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key, const float* quickRenderDelay){
	int fogSumOfWeights = 0;
	for(int i = 0; i < (int)FogType::Amt; ++i){
		fogSumOfWeights += fogWeights[i];
	}
	if(fogSumOfWeights == 0){
		(void)puts("fogSumOfWeights == 0");
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

				if(quickRenderDelay != nullptr){
					float currTime = 0.0f;
					float delay = FLT_MAX;

					while(quickRenderDelay != nullptr && currTime < delay){
						currTime += (float)timer.getElapsedTime();

						if(!isKeySpace && App::Key(VK_SPACE)){
							quickRenderDelay = nullptr;
							isKeySpace = true;
						} else if(isKeySpace && !App::Key(VK_SPACE)){
							isKeySpace = false;
						}

						const int result = App::Key(VK_OEM_COMMA) - App::Key(VK_OEM_PERIOD);
						delay = *quickRenderDelay * (result == 0 ? 1.0f : 1.25f + 0.75f * result);
					}
				}

				break;
			}
			val -= fogWeight;
		}
	}

	fogLayer[startRow * gridCols + startCol] = FogType::Inexistent;
	fogLayer[gridTotalCells - 1] = FogType::Inexistent; //End
}

void Sim::GenTileLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key, const float* quickRenderDelay){
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

	if(quickRenderDelay != nullptr){
		float currTime = 0.0f;
		float delay = FLT_MAX;

		while(quickRenderDelay != nullptr && currTime < delay){
			currTime += (float)timer.getElapsedTime();

			if(!isKeySpace && App::Key(VK_SPACE)){
				quickRenderDelay = nullptr;
				isKeySpace = true;
			} else if(isKeySpace && !App::Key(VK_SPACE)){
				isKeySpace = false;
			}

			const int result = App::Key(VK_OEM_COMMA) - App::Key(VK_OEM_PERIOD);
			delay = *quickRenderDelay * (result == 0 ? 1.0f : 1.25f + 0.75f * result);
		}
	}

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

			if(quickRenderDelay != nullptr){
				float currTime = 0.0f;
				float delay = FLT_MAX;

				while(quickRenderDelay != nullptr && currTime < delay){
					currTime += (float)timer.getElapsedTime();

					if(!isKeySpace && App::Key(VK_SPACE)){
						quickRenderDelay = nullptr;
						isKeySpace = true;
					} else if(isKeySpace && !App::Key(VK_SPACE)){
						isKeySpace = false;
					}

					const int result = App::Key(VK_OEM_COMMA) - App::Key(VK_OEM_PERIOD);
					delay = *quickRenderDelay * (result == 0 ? 1.0f : 1.25f + 0.75f * result);
				}
			}

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

	tileLayer[gridRows * gridCols - 1] = TileType::Empty; //End

	if(quickRenderDelay != nullptr){
		float currTime = 0.0f;
		float delay = FLT_MAX;

		while(quickRenderDelay != nullptr && currTime < delay){
			currTime += (float)timer.getElapsedTime();

			if(!isKeySpace && App::Key(VK_SPACE)){
				quickRenderDelay = nullptr;
				isKeySpace = true;
			} else if(isKeySpace && !App::Key(VK_SPACE)){
				isKeySpace = false;
			}

			const int result = App::Key(VK_OEM_COMMA) - App::Key(VK_OEM_PERIOD);
			delay = *quickRenderDelay * (result == 0 ? 1.0f : 1.25f + 0.75f * result);
		}
	}
}

void Sim::RefineTileLayer(const int gridRows, const int gridCols, const unsigned int key, const float* quickRenderDelay){
	int tileSumOfWeights = 0;
	for(int i = 0; i < (int)TileType::Amt; ++i){
		tileSumOfWeights += tileWeights[i];
	}
	if(tileSumOfWeights == 0){
		(void)puts("tileSumOfWeights == 0");
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

					if(quickRenderDelay != nullptr){
						float currTime = 0.0f;
						float delay = FLT_MAX;

						while(quickRenderDelay != nullptr && currTime < delay){
							currTime += (float)timer.getElapsedTime();

							if(!isKeySpace && App::Key(VK_SPACE)){
								quickRenderDelay = nullptr;
								isKeySpace = true;
							} else if(isKeySpace && !App::Key(VK_SPACE)){
								isKeySpace = false;
							}

							const int result = App::Key(VK_OEM_COMMA) - App::Key(VK_OEM_PERIOD);
							delay = *quickRenderDelay * (result == 0 ? 1.0f : 1.25f + 0.75f * result);
						}
					}

					break;
				}
				val -= tileWeight;
			}
		}
	}
}

void Sim::MakeRadialHoleInTileLayer(const int gridRows, const int gridCols, const int row, const int col, const int radius, const float* quickRenderDelay){
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

		if(quickRenderDelay != nullptr){
			float currTime = 0.0f;
			float delay = FLT_MAX;

			while(quickRenderDelay != nullptr && currTime < delay){
				currTime += (float)timer.getElapsedTime();

				if(!isKeySpace && App::Key(VK_SPACE)){
					quickRenderDelay = nullptr;
					isKeySpace = true;
				} else if(isKeySpace && !App::Key(VK_SPACE)){
					isKeySpace = false;
				}

				const int result = App::Key(VK_OEM_COMMA) - App::Key(VK_OEM_PERIOD);
				delay = *quickRenderDelay * (result == 0 ? 1.0f : 1.25f + 0.75f * result);
			}
		}
	}
}

void Sim::Update(const double dt){
	turnElapsedTime += (float)dt;
	timeOfDayElapsedTime += (float)dt;

	if(turnElapsedTime >= turnDuration){
		++turn;
		turnElapsedTime = 0.0f;
	}
	if(timeOfDayElapsedTime >= timeOfDayDuration){
		timeOfDay = (TimeOfDay)Math::RandIntMinMax((int)TimeOfDay::Day, (int)TimeOfDay::Amt - 1);
		timeOfDayElapsedTime = 0.0f;
	}
}