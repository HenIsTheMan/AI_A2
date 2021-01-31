Sim* StateChaseKnight::sim = nullptr;
Entity** StateChaseKnight::entityMoving = nullptr;
AStar<Vector3, float>* StateChaseKnight::myAStar = nullptr;
std::vector<Vector3>* StateChaseKnight::myShortestPath = nullptr;
HexGrid<float>::GridType StateChaseKnight::gridType = HexGrid<float>::GridType::Amt;
int StateChaseKnight::gridRows = 0;
int StateChaseKnight::gridCols = 0;
int StateChaseKnight::selectedRow = 0;
int StateChaseKnight::selectedCol = 0;
int StateChaseKnight::selectedTargetRow = 0;
int StateChaseKnight::selectedTargetCol = 0;

void StateChaseKnight::Enter(Entity* const entity){
	const std::vector<Entity*>& entityLayer = sim->GetEntityLayer();

	myAStar->Reset();
	myShortestPath->clear();

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			const int cost = (int)tileCosts[(int)sim->GetTileLayer()[r * gridCols + c]];

			(void)myAStar->CreateNode(CreateAStarNodeParams<Vector3, float>{
				cost < 0 || (!(r == selectedRow && c == selectedCol) && entityLayer[r * gridCols + c] != nullptr) ? AStarNodeType::Inaccessible : AStarNodeType::Accessible,
					'(' + std::to_string(c) + ", " + std::to_string(r) + ')' + " Cost: " + std::to_string(cost),
					(float)cost,
					Vector3((float)c, (float)r, 0.0f),
			});
		}
	}

	myAStar->SetStart(Vector3((float)selectedCol, (float)selectedRow, 0.0f));
	myAStar->SetEnd(Vector3((float)selectedTargetCol, (float)selectedTargetRow, 0.0f));
	if(gridType == HexGrid<float>::GridType::FlatTop){
		myAStar->SetNeighboursForHexGridFlatTop(
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3((float)gridCols - 1.0f, 0.0f, 0.0f),
			Vector3(0.0f, (float)gridRows - 1.0f, 0.0f),
			Vector3((float)gridCols - 1.0f, (float)gridRows - 1.0f, 0.0f),
			1.0f,
			1.0f
		);
	} else{
		myAStar->SetNeighboursForHexGridSharpTop(
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3((float)gridCols - 1.0f, 0.0f, 0.0f),
			Vector3(0.0f, (float)gridRows - 1.0f, 0.0f),
			Vector3((float)gridCols - 1.0f, (float)gridRows - 1.0f, 0.0f),
			1.0f,
			1.0f
		);
	}

	if(myAStar->CalcShortestPath()){
		myAStar->PrintPath();

		const std::vector<AStarNode<Vector3, float>*>& shortestPath = myAStar->GetShortestPath();
		for(const AStarNode<Vector3, float>* const node: shortestPath){
			myShortestPath->emplace_back(node->GetPos());
		}

		entity->im_Attribs.im_GridCellTargetLocalPos = myShortestPath->front();
		entity->im_Attribs.im_GridCellStartLocalPos = entity->im_Attribs.im_LocalPos;
		myShortestPath->erase(myShortestPath->begin());
	}

	sim->OnEntityDeactivated(gridCols, (int)entity->im_Attribs.im_LocalPos.y, (int)entity->im_Attribs.im_LocalPos.x);
	selectedRow = selectedCol = -1;
}

void StateChaseKnight::Update(Entity* const entity, const double dt){
	Vector3& entityLocalPos = entity->im_Attribs.im_LocalPos;
	const Vector3 diff = entity->im_Attribs.im_GridCellTargetLocalPos - entityLocalPos;
	const float dist = diff.Length();
	if(!(dist <= Math::EPSILON && -dist <= Math::EPSILON)){
		entityLocalPos = entityLocalPos + 4.0f * diff.Normalized() * (float)dt;
	}

	if((entity->im_Attribs.im_GridCellTargetLocalPos - entityLocalPos).LengthSquared() < 4.0f * (float)dt * 4.0f * (float)dt){
		entityLocalPos = Vector3(
			roundf(entityLocalPos.x),
			roundf(entityLocalPos.y),
			roundf(entityLocalPos.z)
		); //Snap entity's local pos

		if(gridType == HexGrid<float>::GridType::FlatTop){
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Left:
					if(((int)entity->im_Attribs.im_GridCellStartLocalPos.x & 1) == 1){
						entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DL;
					} else{
						entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UL;
					}
					break;
				case Obj::EntityFacingDir::Right:
					if(((int)entity->im_Attribs.im_GridCellStartLocalPos.x & 1) == 1){
						entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DR;
					} else{
						entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UR;
					}
					break;
				case Obj::EntityFacingDir::UL:
				case Obj::EntityFacingDir::DL:
					entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Left;
					break;
				case Obj::EntityFacingDir::UR:
				case Obj::EntityFacingDir::DR:
					entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Right;
					break;
			}
		} else{
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					if(((int)entity->im_Attribs.im_GridCellStartLocalPos.y & 1) == 1){
						entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UL;
					} else{
						entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UR;
					}
					break;
				case Obj::EntityFacingDir::Down:
					if(((int)entity->im_Attribs.im_GridCellStartLocalPos.y & 1) == 1){
						entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DL;
					} else{
						entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DR;
					}
					break;
				case Obj::EntityFacingDir::UL:
				case Obj::EntityFacingDir::UR:
					entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Up;
					break;
				case Obj::EntityFacingDir::DL:
				case Obj::EntityFacingDir::DR:
					entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Down;
					break;
			}
		}

		if(myShortestPath->empty()){
			selectedRow = (int)entityLocalPos.y;
			selectedCol = (int)entityLocalPos.x;

			selectedTargetRow = selectedTargetCol = -1;
			sim->OnEntityActivated(gridCols, entity);
			*entityMoving = nullptr;
		} else{
			entity->im_Attribs.im_GridCellTargetLocalPos = myShortestPath->front();
			entity->im_Attribs.im_GridCellStartLocalPos = entityLocalPos;
			myShortestPath->erase(myShortestPath->begin());
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

void StateChaseKnight::Exit(Entity* const entity){
}