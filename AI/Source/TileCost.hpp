enum struct TileCost: int{
	InvalidCost = -999,
	WallCost = -1,
	EmptyCost,
	SoilCost = 1,
	FireCost = 0,
	WaterCost = 3,
	GrassCost = 2,
	MudCost = 4,
	Amt = 8
};