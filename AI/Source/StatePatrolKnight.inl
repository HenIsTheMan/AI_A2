Sim* StatePatrolKnight::sim = nullptr;
std::vector<Vector3> StatePatrolKnight::myVec = std::vector<Vector3>();
std::vector<bool> StatePatrolKnight::visited = std::vector<bool>();
int StatePatrolKnight::gridRows = 0;
int StatePatrolKnight::gridCols = 0;

void StatePatrolKnight::Enter(Entity* const entity){
	entity->im_Attribs.im_PatrolRange = 20; //??
	myVec.emplace_back(entity->im_Attribs.im_LocalPos);
	//reserve??
}

void StatePatrolKnight::Update(Entity* const entity, const double dt){
	if(myVec.empty()){
		return;
	}

	const Vector3 localPos = entity->im_Attribs.im_LocalPos;
	const std::vector<TileType>& tileLayer = sim->GetTileLayer();

	int nextIndex = 0;
	myVec.emplace_back(localPos);
	visited[(int)localPos.y * gridCols + (int)localPos.x] = true;

	for(int i = 1; i < (int)Obj::EntityFacingDir::Amt; ++i){ //Exclude EntityFacingDir::Invalid
		Vector3 next = localPos;
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
			if((int)tileLayer[nextIndex] >= 0){
				entity->im_Attribs.im_GridCellTargetLocalPos = myVec.back();
				entity->im_Attribs.im_GridCellStartLocalPos = localPos; //??
				return;
			}
		}
	}

	myVec.pop_back();
	if(!myVec.empty()){
		entity->im_Attribs.im_GridCellTargetLocalPos = myVec.back();
		entity->im_Attribs.im_GridCellStartLocalPos = localPos; //??
		myVec.pop_back();
	}
}

void StatePatrolKnight::Exit(Entity* const entity){
}