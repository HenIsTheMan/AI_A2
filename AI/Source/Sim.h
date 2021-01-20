#include <vector>

#include "TimeOfDay.hpp"

#include "FogType.hpp"
#include "TileType.hpp"

class Sim final{
public:
	Sim();
	~Sim() = default;

	void Start(const int gridRows, const int gridCols, const int startRow, const int startCol, const unsigned int key);

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

	std::vector<FogType> fogLayer;
	std::vector<TileType> tileLayer;

	void GenFogLayer(const int gridRows, const int gridCols, const int startRow, const int startCol);
	void GenTileLayer(const int gridRows, const int gridCols, const int startRow, const int startCol);
	void RefineTileLayer(const int gridRows, const int gridCols);
};