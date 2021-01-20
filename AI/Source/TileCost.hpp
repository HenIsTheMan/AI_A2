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

static TileCost tileCosts[(int)TileCost::Amt + 1]{
	TileCost::InvalidCost,
	TileCost::WallCost,
	TileCost::EmptyCost,
	TileCost::SoilCost,
	TileCost::FireCost,
	TileCost::WaterCost,
	TileCost::GrassCost,
	TileCost::MudCost,
	TileCost::Amt
};