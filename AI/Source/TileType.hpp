#pragma once

enum struct TileType: int{
	Invalid,
	Wall,
	Empty,
	Soil,
	Fire, //Can burn Grass to spread, Can spread naturally, can spawn naturally during the day
	Water, //Can spawn with rain
	Grass, //Formed from Soil by Water and day, can spread with water and/or rain, can wither without water and/or rain
	Mud,
	Amt
};