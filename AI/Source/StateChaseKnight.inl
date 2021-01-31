Sim* StateChaseKnight::sim = nullptr;
Entity* StateChaseKnight::entityMoving = nullptr;
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

		entityMoving->im_Attribs.im_GridCellTargetLocalPos = myShortestPath->front();
		entityMoving->im_Attribs.im_GridCellStartLocalPos = entityMoving->im_Attribs.im_LocalPos;
		myShortestPath->erase(myShortestPath->begin());
	}

	sim->OnEntityDeactivated(gridCols, (int)entityMoving->im_Attribs.im_LocalPos.y, (int)entityMoving->im_Attribs.im_LocalPos.x);
	selectedRow = selectedCol = -1;
}

void StateChaseKnight::Update(Entity* const entity, const double dt){
}

void StateChaseKnight::Exit(Entity* const entity){
}