#include "Sim.h"

Sim::Sim():
	gameSpd(1.0f),
	timeOfDay(TimeOfDay::Amt),
	fogLayer(),
	tileLayer()
{
}

void Sim::Start(const int gridRows, const int gridCols){
	GenMapLayers(gridRows, gridCols);
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

void Sim::GenMapLayers(const int gridRows, const int gridCols){
	const int gridTotalCells = gridRows * gridCols;

	fogLayer.reserve(gridTotalCells);
	tileLayer.reserve(gridTotalCells);

	for(int i = 0; i < gridTotalCells; ++i){
		fogLayer.emplace_back(FogType::Inexistent);
		tileLayer.emplace_back(TileType::Mud);
	}
}