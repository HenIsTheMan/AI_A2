Sim* StatePatrolKnight::sim = nullptr;
std::vector<Vector3> StatePatrolKnight::myPath = std::vector<Vector3>();
std::vector<Vector3> StatePatrolKnight::myVec = std::vector<Vector3>();
std::vector<bool> StatePatrolKnight::visited = std::vector<bool>();
int StatePatrolKnight::gridRows = 0;
int StatePatrolKnight::gridCols = 0;

void StatePatrolKnight::Enter(Entity* const entity){
	entity->im_Attribs.im_PatrolRange = 20; //??

	const int gridTotalCells = gridRows * gridCols;
	myPath.reserve(gridTotalCells);
	myVec.reserve(gridTotalCells);
	visited.reserve(gridTotalCells);
	visited.resize(gridTotalCells);

	myVec.emplace_back(entity->im_Attribs.im_LocalPos);

	entity->im_Attribs.im_GridCellTargetLocalPos = entity->im_Attribs.im_LocalPos;
	entity->im_Attribs.im_GridCellStartLocalPos = entity->im_Attribs.im_LocalPos;

	sim->OnEntityDeactivated(gridCols, (int)entity->im_Attribs.im_LocalPos.y, (int)entity->im_Attribs.im_LocalPos.x);
	//*selectedRow = *selectedCol = -1;
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

		if(myVec.empty()){
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
				if((int)tileCosts[(int)tileLayer[nextIndex]] >= 0){
					entity->im_Attribs.im_LocalPos = next;
					entity->im_Attribs.im_GridCellTargetLocalPos = next;
					entity->im_Attribs.im_GridCellStartLocalPos = entityLocalPos;
					return;
				}
			}
		}

		myVec.pop_back();
		if(!myVec.empty()){
			//myPath.emplace_back();
			//entity->im_Attribs.im_LocalPos = myVec.back();
			entity->im_Attribs.im_GridCellTargetLocalPos = myVec.back();
			entity->im_Attribs.im_GridCellStartLocalPos = entityLocalPos;
			myVec.pop_back();
		}
	}
}

void StatePatrolKnight::Exit(Entity* const entity){
}