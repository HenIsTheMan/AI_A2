#include <vector>

#include "TimeOfDay.hpp"

#include "FogType.hpp"
#include "TileType.hpp"

class Sim final{
public:
	struct{
		float spd = 0.0f;
		float turnTime = 0.0f;
		int turn = 0;
		TimeOfDay timeOfDay = TimeOfDay::Amt;
	};

	Sim();
	~Sim() = default;

	void ChangeFogWeight(const int index, const int fogWeight);
	void ChangeTileWeight(const int index, const int tileWeight);

	void GenFogLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key);
	void GenTileLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key);
	void RefineTileLayer(const int gridRows, const int gridCols, const unsigned int key);
	void MakeRadialHoleInTileLayer(const int gridRows, const int gridCols, const int row, const int col, const int radius);

	//* Getters
	const std::vector<FogType>& GetFogLayer() const;
	const std::vector<TileType>& GetTileLayer() const;
	//*/
private:
	int fogWeights[(int)FogType::Amt];
	int tileWeights[(int)TileType::Amt];

	std::vector<FogType> fogLayer;
	std::vector<TileType> tileLayer;
};