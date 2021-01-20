#include <vector>

#include "TimeOfDay.hpp"

#include "FogType.hpp"
#include "TileType.hpp"

class Sim final{
public:
	Sim();
	~Sim() = default;

	void ChangeWeight(const int index, const int weight);
	void GenFogLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key);
	void GenTileLayer(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key);
	void RefineTileLayer(const int gridRows, const int gridCols, const unsigned int key);
	void MakeRadialHoleInTileLayer(const int gridRows, const int gridCols, const int row, const int col, const int radius);

	float& RetrieveGameSpd();
	TimeOfDay& RetrieveTimeOfDay();
	std::vector<FogType>& RetrieveFogLayer();
	std::vector<TileType>& RetrieveTileLayer();

	//* Getters
	float GetGameSpd() const;
	TimeOfDay GetTimeOfDay() const;
	const std::vector<FogType>& GetFogLayer() const;
	const std::vector<TileType>& GetTileLayer() const;
	//*/

	//* Setters
	void SetGameSpd(const float gameSpd);
	void SetTimeOfDay(const TimeOfDay timeOfDay);
	//*/
private:
	float gameSpd;
	TimeOfDay timeOfDay;

	int weights[(int)TileType::Amt];

	std::vector<FogType> fogLayer;
	std::vector<TileType> tileLayer;
};