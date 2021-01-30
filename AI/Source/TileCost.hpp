#pragma once

enum struct TileCost: int{
	InvalidCost = -999,
	WallCost = -1,
	EmptyCost = 1,
	SoilCost = 2,
	FireCost = 1,
	WaterCost = 4,
	GrassCost = 3,
	MudCost = 5,
	Amt = 8
};

static TileCost tileCosts[(int)TileCost::Amt]{
	TileCost::InvalidCost,
	TileCost::WallCost,
	TileCost::EmptyCost,
	TileCost::SoilCost,
	TileCost::FireCost,
	TileCost::WaterCost,
	TileCost::GrassCost,
	TileCost::MudCost,
};