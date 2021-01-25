namespace Obj{
	template <class T>
	T FindValidLocalPos(const int gridCols, const Sim* const sim){
		assert(false);
		return T();
	}

	template <>
	Vector3 FindValidLocalPos(const int gridCols, const Sim* const sim){
		const std::vector<::Entity*>& entityLayer = sim->GetEntityLayer();
		const std::vector<TileType>& tileLayer = sim->GetTileLayer();
		int index = -1;
		int tileCostIndex = -1;
		const int gridTotalCells = (int)tileLayer.size();

		do{
			index = Math::RandIntMinMax(0, gridTotalCells - 1);
			tileCostIndex = (int)tileLayer[index];
		} while(entityLayer[index] != nullptr || (tileCostIndex >= 0 && tileCostIndex <= (int)TileCost::Amt - 1 && (int)tileCosts[tileCostIndex] < 0));

		return Vector3(float(index % gridCols), float(index / gridCols), 0.0f);
	}
}