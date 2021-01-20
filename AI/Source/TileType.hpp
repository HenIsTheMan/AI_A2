enum struct TileType: int{
	Invalid,
	Wall,
	Empty,
	Soil,
	Fire, //Can burn Grass, Can be put out by rain
	Water, //Formed from Empty by rain, Can flow
	Grass, //Formed from Soil by Water and day
	Mud, //Formed from Soil by rain
	Amt
};