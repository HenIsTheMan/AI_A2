Sim* StatePatrolKnight::sim = nullptr;
std::vector<Vector3> StatePatrolKnight::myVec = std::vector<Vector3>();
std::vector<bool> StatePatrolKnight::visited = std::vector<bool>();
int StatePatrolKnight::gridRows = 0;
int StatePatrolKnight::gridCols = 0;
int* StatePatrolKnight::selectedRow = nullptr;
int* StatePatrolKnight::selectedCol = nullptr;

void StatePatrolKnight::Enter(Entity* const entity){
	entity->im_Attribs.im_PatrolRange = 20; //??

	const int gridTotalCells = gridRows * gridCols;
	myVec.reserve(gridTotalCells);
	visited.reserve(gridTotalCells);
	visited.resize(gridTotalCells);

	myVec.emplace_back(entity->im_Attribs.im_LocalPos);

	entity->im_Attribs.im_GridCellTargetLocalPos = entity->im_Attribs.im_LocalPos;
	entity->im_Attribs.im_GridCellStartLocalPos = entity->im_Attribs.im_LocalPos;

	sim->OnEntityDeactivated(gridCols, (int)entity->im_Attribs.im_LocalPos.y, (int)entity->im_Attribs.im_LocalPos.x);
	*selectedRow = *selectedCol = -1;
}

void StatePatrolKnight::Update(Entity* const entity, const double dt){
	const std::vector<TileType>& tileLayer = sim->GetTileLayer();
	Vector3& entityLocalPos = entity->im_Attribs.im_LocalPos;
	const Vector3 diff = entity->im_Attribs.im_GridCellTargetLocalPos - entityLocalPos;
	const float dist = diff.Length();

	if(!(dist <= Math::EPSILON && -dist <= Math::EPSILON)){
		entityLocalPos = entityLocalPos + entity->im_Attribs.im_Spd * diff.Normalized() * (float)dt;
	}

	if((entity->im_Attribs.im_GridCellTargetLocalPos - entityLocalPos).LengthSquared() < entity->im_Attribs.im_Spd * (float)dt * entity->im_Attribs.im_Spd * (float)dt){
		entityLocalPos = Vector3(
			roundf(entityLocalPos.x),
			roundf(entityLocalPos.y),
			roundf(entityLocalPos.z)
		); //Snap entity's local pos

		if(myVec.empty()){ //??
			return;
		}

		int nextIndex = 0;
		myVec.emplace_back(entityLocalPos);
		visited[(int)entityLocalPos.y * gridCols + (int)entityLocalPos.x] = true;

		for(int i = 1; i < (int)Obj::EntityFacingDir::Amt; ++i){ //Exclude EntityFacingDir::Invalid
			Vector3 next = entityLocalPos;
			switch(Obj::entityFacingDirs[i]){
				case Obj::EntityFacingDir::Up:
					++next.y;
					break;
				case Obj::EntityFacingDir::Down:
					--next.y;
					break;
				case Obj::EntityFacingDir::Left:
					--next.x;
					break;
				case Obj::EntityFacingDir::Right:
					++next.x;
					break;
				case Obj::EntityFacingDir::UL:
					--next.x;
					++next.y;
					break;
				case Obj::EntityFacingDir::UR:
					++next.x;
					++next.y;
					break;
				case Obj::EntityFacingDir::DL:
					--next.x;
					--next.y;
					break;
				case Obj::EntityFacingDir::DR:
					++next.x;
					--next.y;
					break;
			}

			if(next.x < 0 || next.x > gridCols - 1 || next.y < 0 || next.y > gridRows - 1){
				continue;
			}

			nextIndex = (int)next.y * gridCols + (int)next.x; //Update nextIndex
			if(!visited[nextIndex]){
				const int index = (int)tileLayer[nextIndex];
				if(index >= 0 && index <= (int)TileCost::Amt - 1 && (int)tileCosts[index] >= 0){
					entity->im_Attribs.im_GridCellTargetLocalPos = next;
					entity->im_Attribs.im_GridCellStartLocalPos = entityLocalPos;
					return;
				}
			}
		}

		myVec.pop_back();
		if(!myVec.empty()){
			entity->im_Attribs.im_GridCellTargetLocalPos = myVec.back();
			entity->im_Attribs.im_GridCellStartLocalPos = entityLocalPos;
			myVec.pop_back();
		}
	} else{
		if((int)entity->im_Attribs.im_GridCellStartLocalPos.x == (int)entity->im_Attribs.im_GridCellTargetLocalPos.x){

			if(entity->im_Attribs.im_GridCellTargetLocalPos.y > entity->im_Attribs.im_GridCellStartLocalPos.y){
				entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Up;
			} else{
				entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Down;
			}

		} else if((int)entity->im_Attribs.im_GridCellStartLocalPos.y == (int)entity->im_Attribs.im_GridCellTargetLocalPos.y){

			if(entity->im_Attribs.im_GridCellTargetLocalPos.x > entity->im_Attribs.im_GridCellStartLocalPos.x){
				entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Right;
			} else{
				entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Left;
			}

		} else{

			if(entity->im_Attribs.im_GridCellTargetLocalPos.y > entity->im_Attribs.im_GridCellStartLocalPos.y){
				if(entity->im_Attribs.im_GridCellTargetLocalPos.x > entity->im_Attribs.im_GridCellStartLocalPos.x){
					entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UR;
				} else{
					entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UL;
				}
			} else{
				if(entity->im_Attribs.im_GridCellTargetLocalPos.x > entity->im_Attribs.im_GridCellStartLocalPos.x){
					entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DR;
				} else{
					entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DL;
				}
			}

		}
	}
}

void StatePatrolKnight::Exit(Entity* const entity){
}