namespace Obj{
	EntityFacingDir ChooseRandFacingDir(const bool isFlatTop, const int row, const int col){
		if(isFlatTop){
			if((col & 1) == 1){
				switch(Math::RandIntMinMax(1, 6)){
					case 1:
						return EntityFacingDir::Up;
						break;
					case 2:
						return EntityFacingDir::Down;
						break;
					case 3:
						return EntityFacingDir::Left;
						break;
					case 4:
						return EntityFacingDir::Right;
						break;
					case 5:
						return EntityFacingDir::UL;
						break;
					case 6:
						return EntityFacingDir::UR;
						break;
				}
			} else{
				switch(Math::RandIntMinMax(1, 6)){
					case 1:
						return EntityFacingDir::Up;
						break;
					case 2:
						return EntityFacingDir::Down;
						break;
					case 3:
						return EntityFacingDir::Left;
						break;
					case 4:
						return EntityFacingDir::Right;
						break;
					case 5:
						return EntityFacingDir::DL;
						break;
					case 6:
						return EntityFacingDir::DR;
						break;
				}
			}
		} else{
			if((row & 1) == 1){
				switch(Math::RandIntMinMax(1, 6)){
					case 1:
						return EntityFacingDir::Up;
						break;
					case 2:
						return EntityFacingDir::Down;
						break;
					case 3:
						return EntityFacingDir::Left;
						break;
					case 4:
						return EntityFacingDir::Right;
						break;
					case 5:
						return EntityFacingDir::DR;
						break;
					case 6:
						return EntityFacingDir::UR;
						break;
				}
			} else{
				switch(Math::RandIntMinMax(1, 6)){
					case 1:
						return EntityFacingDir::Up;
						break;
					case 2:
						return EntityFacingDir::Down;
						break;
					case 3:
						return EntityFacingDir::Left;
						break;
					case 4:
						return EntityFacingDir::Right;
						break;
					case 5:
						return EntityFacingDir::DL;
						break;
					case 6:
						return EntityFacingDir::UL;
						break;
				}
			}
		}

		(void)puts("Returning EntityFacingDir::Invalid!");
		return EntityFacingDir::Invalid;
	}

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