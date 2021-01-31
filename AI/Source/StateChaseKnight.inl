Sim* StateChaseKnight::sim = nullptr;
Entity** StateChaseKnight::entityMoving = nullptr;
AStar<Vector3, float>* StateChaseKnight::myAStar = nullptr;
std::vector<Vector3>* StateChaseKnight::myShortestPath = nullptr;
HexGrid<float>::GridType StateChaseKnight::gridType = HexGrid<float>::GridType::Amt;
int StateChaseKnight::gridRows = 0;
int StateChaseKnight::gridCols = 0;
int* StateChaseKnight::selectedRow = nullptr;
int* StateChaseKnight::selectedCol = nullptr;
int* StateChaseKnight::selectedTargetRow = nullptr;
int* StateChaseKnight::selectedTargetCol = nullptr;
int* StateChaseKnight::creditsPlayer = nullptr;
int* StateChaseKnight::creditsAI = nullptr;

void StateChaseKnight::Enter(Entity* const entity){
	myAStar->Reset();
	myShortestPath->clear();

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			const int cost = (int)tileCosts[(int)sim->GetTileLayer()[r * gridCols + c]];

			(void)myAStar->CreateNode(CreateAStarNodeParams<Vector3, float>{
				cost < 0 || sim->GetEntityLayer()[r * gridCols + c] != nullptr
					? AStarNodeType::Inaccessible
					: AStarNodeType::Accessible,
				'(' + std::to_string(c) + ", " + std::to_string(r) + ')' + " Cost: " + std::to_string(cost),
				(float)cost,
				Vector3((float)c, (float)r, 0.0f),
			});
		}
	}

	myAStar->SetStart(Vector3((float)*selectedCol, (float)*selectedRow, 0.0f));
	myAStar->SetEnd(Vector3((float)*selectedTargetCol, (float)*selectedTargetRow, 0.0f));
	myAStar->MakeStartAccessible();
	myAStar->MakeEndAccessible();

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
		const int shortestPathSize = shortestPath.size();
		for(int i = 1; i < shortestPathSize; ++i){ //Exclude start node
			myShortestPath->emplace_back(shortestPath[i]->GetPos());
		}

		entity->im_Attribs.im_GridCellTargetLocalPos = entity->im_Attribs.im_LocalPos;
		entity->im_Attribs.im_GridCellStartLocalPos = entity->im_Attribs.im_LocalPos;
	}

	sim->OnEntityDeactivated(gridCols, (int)entity->im_Attribs.im_LocalPos.y, (int)entity->im_Attribs.im_LocalPos.x);
	*selectedRow = *selectedCol = -1;
}

void StateChaseKnight::Update(Entity* const entity, const double dt){
	if(entity->im_Attribs.im_CurrHealth <= 0.0f){
		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateDeadKnight);
		return;
	}

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
			*selectedRow = (int)entityLocalPos.y;
			*selectedCol = (int)entityLocalPos.x;

			*selectedTargetRow = *selectedTargetCol = -1;

			entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleKnight);
			sim->OnEntityActivated(gridCols, entity);

			*entityMoving = nullptr;
		} else{
			const Vector3 targetLocalPos = myShortestPath->front();
			const int targetIndex = (int)targetLocalPos.y * gridCols + (int)targetLocalPos.x;

			if(sim->GetEntityLayer()[targetIndex] == nullptr){
				const int tileCost = (int)tileCosts[(int)sim->GetTileLayer()[targetIndex]];

				if((sim->turn == SimTurn::Player && *creditsPlayer >= tileCost)
					|| (sim->turn == SimTurn::AI && *creditsAI >= tileCost)){

					if(sim->turn == SimTurn::Player){
						*creditsPlayer -= tileCost;
					} else if(sim->turn == SimTurn::AI){
						*creditsAI -= tileCost;
					}
					entity->im_Attribs.im_GridCellTargetLocalPos = targetLocalPos;
					entity->im_Attribs.im_GridCellStartLocalPos = entityLocalPos;
					myShortestPath->erase(myShortestPath->begin());

				} else{
					*selectedRow = (int)entityLocalPos.y;
					*selectedCol = (int)entityLocalPos.x;

					*selectedTargetRow = *selectedTargetCol = -1;

					entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleKnight);
					sim->OnEntityActivated(gridCols, entity);

					*entityMoving = nullptr;
				}
			} else{
				//* For use in attack state
				entity->im_Attribs.im_GridCellTargetLocalPos = targetLocalPos;
				entity->im_Attribs.im_GridCellStartLocalPos = entityLocalPos;
				//*/

				entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateAttackKnight);
				sim->OnEntityActivated(gridCols, entity);

				*entityMoving = nullptr;
			}
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