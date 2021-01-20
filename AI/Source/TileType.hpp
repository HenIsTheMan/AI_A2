enum struct TileType: int{
	Invalid = -999,
	Wall = -1,
	Empty,
	Soil = 1,
	Fire = 0, //Can burn Grass, Can be put out by rain
	Water = 3, //Formed from Empty by rain, Can flow
	Grass = 2, //Formed from Soil by Water and day
	Mud = 4, //Formed from Soil by rain
	Amt = 8
};