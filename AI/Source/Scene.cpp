#include "Scene.h"

#include "App.h"
#include "GLFW/glfw3.h"

#include "Easing.hpp"

#include "EventAddCredits.h"
#include "EventDespawnEntity.h"
#include "EventResetSelected.h"
#include "ListenerFlags.hpp"

#include "StateAttackKnight.h"
#include "StateChaseKnight.h"
#include "StateDeadKnight.h"
#include "StateIdleKnight.h"
#include "StatePatrolKnight.h"

#include "StateAttackGunner.h"
#include "StateChaseGunner.h"
#include "StateDeadGunner.h"
#include "StateIdleGunner.h"
#include "StatePatrolGunner.h"

#include "StateDeadHealer.h"
#include "StateChaseHealer.h"
#include "StateHealHealer.h"
#include "StateIdleHealer.h"

extern double mouseScrollWheelYOffset;
extern int windowWidth;
extern int windowHeight;

Scene::Scene():
	SceneSupport(),
	canMakeSimMap(false),
	shldRenderTileArt(true),
	shldRenderFog(true),
	customHue(0.0f),
	creditsPlayer(0),
	creditsAI(0),
	canSpawnAmtPlayer(20),
	canSpawnAmtAI(20),
	unitsLeftPlayer(0),
	unitsLeftAI(0),
	gridType(HexGrid<float>::GridType::FlatTop),
	gridCellScaleX(44.0f),
	gridCellScaleY(44.0f),
	gridLineThickness(4.0f),
	gridRows(17),
	gridCols(17),
	gridMinCellScaleX(30.0f),
	gridMaxCellScaleX(70.0f),
	gridMinCellScaleY(30.0f),
	gridMaxCellScaleY(70.0f),
	gridMinLineThickness(0.0f),
	gridMaxLineThickness(10.0f),
	gridMinRows(12),
	gridMinCols(12),
	gridMaxRows(20),
	gridMaxCols(20),
	gridWidth(0.0f),
	gridHeight(0.0f),
	gridOffsetX(0.0f),
	gridOffsetY(0.0f),
	mouseRow(0.0f),
	mouseCol(0.0f),
	selectedRow(-1),
	selectedCol(-1),
	selectedTargetRow(-1),
	selectedTargetCol(-1),
	gridCellFlatToFlatLen(0.0f),
	gridCellSideLen(0.0f),
	gridAltOffsetX(0.0f),
	gridAltOffsetY(0.0f),
	knightSM(new SM()),
	gunnerSM(new SM()),
	healerSM(new SM()),
	sim(new Sim()),
	grid(new HexGrid<float>(HexGrid<float>::GridType::Amt, 0.0f, 0.0f, 0.0f, 0, 0)),
	entityFactory(Obj::EntityFactory<Vector3, float>::RetrieveGlobalObjPtr()),
	entityPool(Obj::ObjPool<Entity>::RetrieveGlobalObjPtr()),
	publisher(Publisher::RetrieveGlobalObjPtr()),
	fogLayer(),
	entitiesToDeactivate(),
	myThread(nullptr),
	entityMoving(nullptr),
	myAStar(),
	myShortestPath(),
	visionLayer()
{
}

Scene::~Scene(){
	if(knightSM != nullptr){
		delete knightSM;
		knightSM = nullptr;
	}

	if(gunnerSM != nullptr){
		delete gunnerSM;
		gunnerSM = nullptr;
	}

	if(healerSM != nullptr){
		delete healerSM;
		healerSM = nullptr;
	}

	if(sim != nullptr){
		delete sim;
		sim = nullptr;
	}

	if(grid != nullptr){
		delete grid;
		grid = nullptr;
	}

	if(entityFactory != nullptr){
		entityFactory->Destroy();
		entityFactory = nullptr;
	}

	if(entityPool != nullptr){
		entityPool->Destroy();
		entityPool = nullptr;
	}

	if(publisher != nullptr){
		publisher->Destroy();
		publisher = nullptr;
	}
	
	if(myThread != nullptr){
		myThread->join();
		delete myThread;
		myThread = nullptr;
	}
}

void Scene::Init(){
	SceneSupport::Init();

	const size_t entityPoolSize = 40000;
	entitiesToDeactivate.reserve(entityPoolSize);
	entityPool->Init(entityPoolSize, entityPoolSize);

	sim->status = SimRuntimeStatus::Waiting;
	sim->spd = 1.0f;
	sim->turnDurationAI = 30.0f;
	sim->turnDurationEnvironment = 10.0f;
	sim->turnDurationPlayer = 30.0f;
	sim->turnElapsedTime = 0.0f;
	sim->turn = Math::RandIntMinMax(0, 1) == 1 ? SimTurn::Player : SimTurn::AI;
	sim->timeOfDayDuration = 4.0f;
	sim->timeOfDayElapsedTime = 0.0f;
	sim->timeOfDay = (SimTimeOfDay)Math::RandIntMinMax((int)SimTimeOfDay::Day, (int)SimTimeOfDay::Amt - 1);

	grid->SetGridType(gridType);
	grid->SetCellScaleX(gridCellScaleX);
	grid->SetCellScaleY(gridCellScaleY);
	grid->SetLineThickness(gridLineThickness);
	grid->SetRows(gridRows);
	grid->SetCols(gridCols);

	knightSM->AddState(new State(StateID::StateAttackKnight, StateAttackKnight::Enter, StateAttackKnight::Update, StateAttackKnight::Exit));
	knightSM->AddState(new State(StateID::StateChaseKnight, StateChaseKnight::Enter, StateChaseKnight::Update, StateChaseKnight::Exit));
	knightSM->AddState(new State(StateID::StateDeadKnight, StateDeadKnight::Enter, StateDeadKnight::Update, StateDeadKnight::Exit));
	knightSM->AddState(new State(StateID::StateIdleKnight, StateIdleKnight::Enter, StateIdleKnight::Update, StateIdleKnight::Exit));
	knightSM->AddState(new State(StateID::StatePatrolKnight, StatePatrolKnight::Enter, StatePatrolKnight::Update, StatePatrolKnight::Exit));

	gunnerSM->AddState(new State(StateID::StateAttackGunner, StateAttackGunner::Enter, StateAttackGunner::Update, StateAttackGunner::Exit));
	gunnerSM->AddState(new State(StateID::StateChaseGunner, StateChaseGunner::Enter, StateChaseGunner::Update, StateChaseGunner::Exit));
	gunnerSM->AddState(new State(StateID::StateDeadGunner, StateDeadGunner::Enter, StateDeadGunner::Update, StateDeadGunner::Exit));
	gunnerSM->AddState(new State(StateID::StateIdleGunner, StateIdleGunner::Enter, StateIdleGunner::Update, StateIdleGunner::Exit));
	gunnerSM->AddState(new State(StateID::StatePatrolGunner, StatePatrolGunner::Enter, StatePatrolGunner::Update, StatePatrolGunner::Exit));

	healerSM->AddState(new State(StateID::StateDeadHealer, StateDeadHealer::Enter, StateDeadHealer::Update, StateDeadHealer::Exit));
	healerSM->AddState(new State(StateID::StateChaseHealer, StateChaseHealer::Enter, StateChaseHealer::Update, StateChaseHealer::Exit));
	healerSM->AddState(new State(StateID::StateHealHealer, StateHealHealer::Enter, StateHealHealer::Update, StateHealHealer::Exit));
	healerSM->AddState(new State(StateID::StateIdleHealer, StateIdleHealer::Enter, StateIdleHealer::Update, StateIdleHealer::Exit));

	entityFactory->SetKnightSM(knightSM);
	entityFactory->SetGunnerSM(gunnerSM);
	entityFactory->SetHealerSM(healerSM);

	publisher->AddListener((long int)ListenerFlags::Scene, this);
}

void Scene::Update(const double updateDt, const double renderDt){
	SceneSupport::Update(updateDt, renderDt);

	OnEvent(FetchEvent(), true);
	UpdateMisc(updateDt);

	switch(sim->status){
		case SimRuntimeStatus::Waiting:
			UpdateSimWaiting(updateDt);
			break;
		case SimRuntimeStatus::MakingTheMap:
			UpdateSimMakingTheMap(updateDt);
			break;
		case SimRuntimeStatus::Ongoing:
			UpdateSimOngoing(updateDt);
			break;
		case SimRuntimeStatus::Ended:
			UpdateSimEnded(updateDt);
			break;
	}

	gridWidth = grid->CalcWidth();
	gridHeight = grid->CalcHeight();
	gridOffsetX = ((float)windowWidth - gridWidth) * 0.5f;
	gridOffsetY = ((float)windowHeight - gridHeight) * 0.5f;

	double mouseX;
	double mouseY;
	App::GetCursorPos(&mouseX, &mouseY);

	const float horizSize = (float)windowWidth * (float)orthoProjectionScaleFactor;
	const float vertSize = (float)windowHeight * (float)orthoProjectionScaleFactor;
	const float leftVal = ((float)windowWidth - horizSize) * 0.5f;
	const float rightVal = leftVal + horizSize;
	const float bottomVal = ((float)windowHeight - vertSize) * 0.5f;
	const float topVal = bottomVal + vertSize;

	const float trueMouseX = (float)mouseX * ((rightVal - leftVal) / (float)windowWidth) + leftVal + im_Cam.pos.x;
	const float trueMouseY = (float)mouseY * ((topVal - bottomVal) / (float)windowHeight) + bottomVal - im_Cam.pos.y;

	float unitX = gridLineThickness;
	float unitY = gridLineThickness;
	if(gridType == HexGrid<float>::GridType::FlatTop){
		unitX += gridCellSideLen * 1.5f;
		unitY += gridCellFlatToFlatLen;
	} else{
		unitX += gridCellFlatToFlatLen;
		unitY += gridCellSideLen * 1.5f;
	}

	if(gridType == HexGrid<float>::GridType::FlatTop){
		mouseCol = std::floorf((trueMouseX - gridOffsetX + unitX * 0.5f) / unitX);
		mouseRow = std::floorf(((float)windowHeight - trueMouseY - gridOffsetY - ((int)mouseCol & 1) * gridAltOffsetY + gridAltOffsetY) / unitY);
	} else{
		mouseRow = std::floorf(((float)windowHeight - trueMouseY - gridOffsetY + unitY * 0.5f) / unitY);
		mouseCol = std::floorf((trueMouseX - gridOffsetX - ((int)mouseRow & 1) * gridAltOffsetX + gridAltOffsetX) / unitX);
	}

	if(mouseRow > gridRows - 1 || mouseCol > gridCols - 1){ //Lame
		mouseRow = mouseCol = -1;
	}

	gridCellFlatToFlatLen = grid->CalcCellFlatToFlatLen();
	gridCellSideLen = grid->CalcCellSideLen();
	gridAltOffsetX = grid->CalcAltOffsetX();
	gridAltOffsetY = grid->CalcAltOffsetY();

	mouseScrollWheelYOffset = 0.0;
}

void Scene::UpdateSimWaiting(const double dt){
	UpdateGridAttribs();

	if(canMakeSimMap && App::IsMousePressed(GLFW_MOUSE_BUTTON_MIDDLE)){
		myThread = new std::thread(&Scene::MakeSimMap, this);
		sim->InitEntityLayer(gridRows, gridCols);

		const int gridTotalCells = gridRows * gridCols;
		fogLayer.reserve(gridTotalCells);
		fogLayer.resize(gridTotalCells);
		myShortestPath.reserve(gridTotalCells);

		StateDeadKnight::entityMoving = &entityMoving;
		StateDeadGunner::entityMoving = &entityMoving;
		StateDeadHealer::entityMoving = &entityMoving;

		const bool isFlatTop = gridType == HexGrid<float>::GridType::FlatTop;
		StateIdleKnight::sm_IsFlatTop = isFlatTop;
		StateIdleGunner::sm_IsFlatTop = isFlatTop;
		StateIdleHealer::sm_IsFlatTop = isFlatTop;

		StateChaseKnight::sim = sim;
		StateChaseKnight::entityMoving = &entityMoving;
		StateChaseKnight::myAStar = &myAStar;
		StateChaseKnight::myShortestPath = &myShortestPath;
		StateChaseKnight::gridType = gridType;
		StateChaseKnight::gridRows = gridRows;
		StateChaseKnight::gridCols = gridCols;
		StateChaseKnight::selectedRow = &selectedRow;
		StateChaseKnight::selectedCol = &selectedCol;
		StateChaseKnight::selectedTargetRow = &selectedTargetRow;
		StateChaseKnight::selectedTargetCol = &selectedTargetCol;
		StateChaseKnight::creditsPlayer = &creditsPlayer;
		StateChaseKnight::creditsAI = &creditsAI;

		StateChaseGunner::sim = sim;
		StateChaseGunner::entityMoving = &entityMoving;
		StateChaseGunner::myAStar = &myAStar;
		StateChaseGunner::myShortestPath = &myShortestPath;
		StateChaseGunner::gridType = gridType;
		StateChaseGunner::gridRows = gridRows;
		StateChaseGunner::gridCols = gridCols;
		StateChaseGunner::selectedRow = &selectedRow;
		StateChaseGunner::selectedCol = &selectedCol;
		StateChaseGunner::selectedTargetRow = &selectedTargetRow;
		StateChaseGunner::selectedTargetCol = &selectedTargetCol;
		StateChaseGunner::creditsPlayer = &creditsPlayer;
		StateChaseGunner::creditsAI = &creditsAI;

		StateChaseHealer::sim = sim;
		StateChaseHealer::entityMoving = &entityMoving;
		StateChaseHealer::myAStar = &myAStar;
		StateChaseHealer::myShortestPath = &myShortestPath;
		StateChaseHealer::gridType = gridType;
		StateChaseHealer::gridRows = gridRows;
		StateChaseHealer::gridCols = gridCols;
		StateChaseHealer::selectedRow = &selectedRow;
		StateChaseHealer::selectedCol = &selectedCol;
		StateChaseHealer::selectedTargetRow = &selectedTargetRow;
		StateChaseHealer::selectedTargetCol = &selectedTargetCol;
		StateChaseHealer::creditsPlayer = &creditsPlayer;
		StateChaseHealer::creditsAI = &creditsAI;

		StateAttackKnight::sim = sim;
		StateAttackKnight::gridCols = gridCols;
		StateAttackKnight::selectedRow = &selectedRow;
		StateAttackKnight::selectedCol = &selectedCol;
		StateAttackKnight::selectedTargetRow = &selectedTargetRow;
		StateAttackKnight::selectedTargetCol = &selectedTargetCol;
		StateAttackKnight::creditsPlayer = &creditsPlayer;
		StateAttackKnight::creditsAI = &creditsAI;

		StateAttackGunner::sim = sim;
		StateAttackGunner::gridCols = gridCols;
		StateAttackGunner::selectedRow = &selectedRow;
		StateAttackGunner::selectedCol = &selectedCol;
		StateAttackGunner::selectedTargetRow = &selectedTargetRow;
		StateAttackGunner::selectedTargetCol = &selectedTargetCol;
		StateAttackGunner::creditsPlayer = &creditsPlayer;
		StateAttackGunner::creditsAI = &creditsAI;

		StateHealHealer::sim = sim;
		StateHealHealer::gridCols = gridCols;
		StateHealHealer::selectedRow = &selectedRow;
		StateHealHealer::selectedCol = &selectedCol;
		StateHealHealer::selectedTargetRow = &selectedTargetRow;
		StateHealHealer::selectedTargetCol = &selectedTargetCol;
		StateHealHealer::creditsPlayer = &creditsPlayer;
		StateHealHealer::creditsAI = &creditsAI;

		StatePatrolKnight::sim = sim;
		StatePatrolKnight::entityMoving = &entityMoving;
		StatePatrolKnight::gridType = gridType;
		StatePatrolKnight::gridRows = gridRows;
		StatePatrolKnight::gridCols = gridCols;
		StatePatrolKnight::selectedRow = &selectedRow;
		StatePatrolKnight::selectedCol = &selectedCol;
		StatePatrolKnight::selectedTargetRow = &selectedTargetRow;
		StatePatrolKnight::selectedTargetCol = &selectedTargetCol;
		StatePatrolKnight::creditsPlayer = &creditsPlayer;
		StatePatrolKnight::creditsAI = &creditsAI;

		StatePatrolGunner::sim = sim;
		StatePatrolGunner::entityMoving = &entityMoving;
		StatePatrolGunner::gridType = gridType;
		StatePatrolGunner::gridRows = gridRows;
		StatePatrolGunner::gridCols = gridCols;
		StatePatrolGunner::selectedRow = &selectedRow;
		StatePatrolGunner::selectedCol = &selectedCol;
		StatePatrolGunner::selectedTargetRow = &selectedTargetRow;
		StatePatrolGunner::selectedTargetCol = &selectedTargetCol;
		StatePatrolGunner::creditsPlayer = &creditsPlayer;
		StatePatrolGunner::creditsAI = &creditsAI;

		canMakeSimMap = false;
	}
}

void Scene::UpdateSimMakingTheMap(const double dt){
	im_Cam.Update(dt);
	if(App::Key('R')){
		orthoProjectionScaleFactor = 1.0;
	}
	orthoProjectionScaleFactor -= mouseScrollWheelYOffset * 0.02; //No need dt
	orthoProjectionScaleFactor = Math::Clamp(orthoProjectionScaleFactor, 0.2, 1.0);
}

void Scene::UpdateSimOngoing(const double dt){
	im_Cam.Update(dt);
	if(App::Key('R')){
		orthoProjectionScaleFactor = 1.0;
	}
	orthoProjectionScaleFactor -= mouseScrollWheelYOffset * 0.02; //No need dt
	orthoProjectionScaleFactor = Math::Clamp(orthoProjectionScaleFactor, 0.2, 1.0);

	switch(sim->turn){
		case SimTurn::AI:
			UpdateSimOngoingTurnAI(dt);
			break;
		case SimTurn::Environment:
			UpdateSimOngoingTurnEnvironment(dt);
			break;
		case SimTurn::Player:
			UpdateSimOngoingTurnPlayer(dt);
			break;
	}

	sim->Update(dt); //Not (dt * sim->spd) as...
	UpdateStates(dt * sim->spd);
	UpdateEntities(dt * sim->spd);

	switch(sim->turn){
		case SimTurn::AI:
			LateUpdateSimOngoingTurnAI(dt);
			break;
		case SimTurn::Environment:
			LateUpdateSimOngoingTurnEnvironment(dt);
			break;
		case SimTurn::Player:
			LateUpdateSimOngoingTurnPlayer(dt);
			break;
	}
	UpdateFog(0.0f);
}

void Scene::UpdateSimEnded(const double dt){
}

void Scene::UpdateSimOngoingTurnAI(const double dt){
	static float decisionBT = 0.0f;
	if(decisionBT <= elapsedTime){
		if(canSpawnAmtAI > 0 && (unitsLeftAI < 3 || Math::RandIntMinMax(1, 10) == 10)){
			sim->OnEntityActivated(gridCols, entityFactory->SpawnRandUnit(gridCols, sim, gridType == HexGrid<float>::GridType::FlatTop));
			--canSpawnAmtAI;
		} else{
			const std::vector<Entity*>& entityLayer = sim->GetEntityLayer();
			const int entityLayerSize = entityLayer.size();

			std::vector<Entity*> entities;
			entities.reserve(entityLayerSize);
			for(int i = 0; i < entityLayerSize; ++i){
				Entity* const entity = entityLayer[i];
				if(entity != nullptr && entity->im_Attribs.im_Team == Obj::EntityTeam::AI){
					entities.emplace_back(entity);
				}
			}
			std::random_shuffle(entities.begin(), entities.end());

			if(!entities.empty()){
				Entity* const selectedEntity = entities[0];

				selectedRow = (int)selectedEntity->im_Attribs.im_LocalPos.y;
				selectedCol = (int)selectedEntity->im_Attribs.im_LocalPos.x;

				selectedTargetRow = selectedTargetCol = -1;

				if(entityMoving == nullptr){
					entityMoving = selectedEntity;
					entityMoving->im_Attribs.im_IdleShldChase = false;
					entityMoving->im_Attribs.im_IdleShldPatrol = true;
				}
			}
		}

		decisionBT = elapsedTime + Math::RandFloatMinMax(0.5f, 1.0f);
	}
}

void Scene::UpdateSimOngoingTurnEnvironment(const double dt){
}

void Scene::UpdateSimOngoingTurnPlayer(const double dt){
	const std::vector<Entity*>& entityLayer = sim->GetEntityLayer();
	const std::vector<TileType>& tileLayer = sim->GetTileLayer();

	static bool isKeyDownSpace = false;
	if(!isKeyDownSpace && App::Key(VK_SPACE)){
		sim->turn = Math::RandIntMinMax(1, 10) <= 4 ? SimTurn::Environment : SimTurn::AI;
		sim->turnElapsedTime = 0.0f;
		creditsAI += 100;
		selectedRow = selectedCol = -1;
		selectedTargetRow = selectedTargetCol = -1;

		//* Idle for...
		for(Entity* const entity: entityLayer){
			if(entity != nullptr && entity->im_Attribs.im_Team == Obj::EntityTeam::Player){
				using Obj::EntityType;

				switch(entity->im_Attribs.im_Type){
					case EntityType::Knight:
						if(entity->im_Attribs.im_CurrState != entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateDeadKnight)){
							entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleKnight);
						}
						break;
					case EntityType::Gunner:
						if(entity->im_Attribs.im_CurrState != entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateDeadGunner)){
							entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleGunner);
						}
						break;
					case EntityType::Healer:
						if(entity->im_Attribs.im_CurrState != entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateDeadHealer)){
							entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleHealer);
						}
						break;
				}
			}
		}
		//*/

		isKeyDownSpace = true;
	} else if(isKeyDownSpace && !App::Key(VK_SPACE)){
		isKeyDownSpace = false;
	}

	static bool isKeyDownQ = false;
	if(!isKeyDownQ && App::Key('Q')){
		if(canSpawnAmtPlayer){
			sim->OnEntityActivated(gridCols, entityFactory->SpawnRandUnit(gridCols, sim, gridType == HexGrid<float>::GridType::FlatTop));
			--canSpawnAmtPlayer;
		}

		isKeyDownQ = true;
	} else if(isKeyDownQ && !App::Key('Q')){
		isKeyDownQ = false;
	}
}

void Scene::LateUpdateSimOngoingTurnAI(const double dt){
}

void Scene::LateUpdateSimOngoingTurnEnvironment(const double dt){
}

void Scene::LateUpdateSimOngoingTurnPlayer(const double dt){
	const std::vector<Entity*>& entityLayer = sim->GetEntityLayer();
	const std::vector<TileType>& tileLayer = sim->GetTileLayer();

	static bool isLMB = false;
	if(!isLMB && App::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT)){
		if(entityMoving == nullptr && mouseRow >= 0 && mouseCol >= 0){
			selectedRow = (int)mouseRow;
			selectedCol = (int)mouseCol;

			selectedTargetRow = selectedTargetCol = -1;
		}

		isLMB = true;
	} else if(isLMB && !App::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT)){
		isLMB = false;
	}

	static bool isRMB = false;
	if(!isRMB && App::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT)){
		const int tileCost = (int)tileCosts[(int)tileLayer[(int)mouseRow * gridCols + (int)mouseCol]];

		if((entityMoving == nullptr
			|| (entityMoving != nullptr
			&& entityMoving->im_Attribs.im_CurrHealth > 0.0f
			&& (selectedTargetRow < 0 || selectedTargetCol < 0)))
			&& mouseRow >= 0 && mouseCol >= 0
			&& selectedRow >= 0 && selectedCol >= 0
			&& (mouseRow != selectedRow || mouseCol != selectedCol)
			&& ((sim->turn == SimTurn::Player && creditsPlayer >= tileCost)
			|| (sim->turn == SimTurn::AI && creditsAI >= tileCost))
		){
			selectedTargetRow = (int)mouseRow;
			selectedTargetCol = (int)mouseCol;

			const int indexSelectedTarget = selectedTargetRow * gridCols + selectedTargetCol;
			Entity* const entitySelected = entityLayer[selectedRow * gridCols + selectedCol];

			if(entitySelected != nullptr
				&& entitySelected->im_Attribs.im_CurrHealth > 0.0f
				&& ((entitySelected->im_Attribs.im_Team == Obj::EntityTeam::Player && sim->turn == SimTurn::Player)
				|| (entitySelected->im_Attribs.im_Team == Obj::EntityTeam::AI && sim->turn == SimTurn::AI))
				&& (int)tileCosts[(int)tileLayer[indexSelectedTarget]] >= (int)TileCost::EmptyCost
				&& !visionLayer[indexSelectedTarget]
			){
				entityMoving = entitySelected;
				entityMoving->im_Attribs.im_IdleShldChase = true;
				entityMoving->im_Attribs.im_IdleShldPatrol = false;
			}
		}

		isRMB = true;
	} else if(isRMB && !App::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT)){
		isRMB = false;
	}

	static bool isKeyP = false;
	if(!isKeyP && App::Key('P')){
		if(entityMoving == nullptr && selectedRow >= 0 && selectedCol >= 0){
			entityMoving = entityLayer[selectedRow * gridCols + selectedCol];
			entityMoving->im_Attribs.im_IdleShldChase = false;
			entityMoving->im_Attribs.im_IdleShldPatrol = true;
		}

		isKeyP = true;
	} else if(isKeyP && !App::Key('P')){
		isKeyP = false;
	}
}

void Scene::UpdateGridAttribs(){
	static bool isKeyDownTilde = false;
	static bool isKeyDown1 = false;
	static bool isKeyDown2 = false;
	static bool isKeyDown3 = false;
	static bool isKeyDown4 = false;
	static bool isKeyDown5 = false;
	static bool isKeyDown6 = false;
	static bool isKeyDown7 = false;
	static bool isKeyDown8 = false;
	static bool isKeyDown9 = false;
	static bool isKeyDown0 = false;

	if(!isKeyDownTilde && App::Key(VK_OEM_3)){
		gridType = gridType == HexGrid<float>::GridType::SharpTop ? HexGrid<float>::GridType::FlatTop : HexGrid<float>::GridType::SharpTop;
		grid->SetGridType(gridType);
		isKeyDownTilde = true;
	} else if(isKeyDownTilde && !App::Key(VK_OEM_3)){
		isKeyDownTilde = false;
	}

	if(!isKeyDown1 && App::Key('1')){
		if(gridCellScaleX < gridMaxCellScaleX){
			grid->SetCellScaleX(++gridCellScaleX);
		}
		if(gridCellScaleY < gridMaxCellScaleY){
			grid->SetCellScaleY(++gridCellScaleY);
		}
		isKeyDown1 = true;
	} else if(isKeyDown1 && !App::Key('1')){
		isKeyDown1 = false;
	}

	if(!isKeyDown2 && App::Key('2')){
		if(gridCellScaleX > gridMinCellScaleX){
			grid->SetCellScaleX(--gridCellScaleX);
		}
		if(gridCellScaleY > gridMinCellScaleY){
			grid->SetCellScaleY(--gridCellScaleY);
		}
		isKeyDown2 = true;
	} else if(isKeyDown2 && !App::Key('2')){
		isKeyDown2 = false;
	}

	if(!isKeyDown3 && App::Key('3')){
		if(gridLineThickness < gridMaxLineThickness){
			gridLineThickness += 0.1f;
			RoundedTo2dp(gridLineThickness);
			grid->SetLineThickness(gridLineThickness);
		}
		isKeyDown3 = true;
	} else if(isKeyDown3 && !App::Key('3')){
		isKeyDown3 = false;
	}

	if(!isKeyDown4 && App::Key('4')){
		if(gridLineThickness > gridMinLineThickness){
			gridLineThickness -= 0.1f;
			RoundedTo2dp(gridLineThickness);
			grid->SetLineThickness(gridLineThickness);
		}
		isKeyDown4 = true;
	} else if(isKeyDown4 && !App::Key('4')){
		isKeyDown4 = false;
	}

	if(!isKeyDown5 && App::Key('5')){
		if(gridRows < gridMaxRows){
			grid->SetRows(++gridRows);
		}
		isKeyDown5 = true;
	} else if(isKeyDown5 && !App::Key('5')){
		isKeyDown5 = false;
	}

	if(!isKeyDown6 && App::Key('6')){
		if(gridRows > gridMinRows){
			grid->SetRows(--gridRows);
		}
		isKeyDown6 = true;
	} else if(isKeyDown6 && !App::Key('6')){
		isKeyDown6 = false;
	}

	if(!isKeyDown7 && App::Key('7')){
		if(gridCols < gridMaxCols){
			grid->SetCols(++gridCols);
		}
		isKeyDown7 = true;
	} else if(isKeyDown7 && !App::Key('7')){
		isKeyDown7 = false;
	}
	if(!isKeyDown8 && App::Key('8')){
		if(gridCols > gridMinCols){
			grid->SetCols(--gridCols);
		}
		isKeyDown8 = true;
	} else if(isKeyDown8 && !App::Key('8')){
		isKeyDown8 = false;
	}
}

void Scene::UpdateMisc(const double dt){
	customHue += (float)dt * 40.0f;
	if(customHue >= 360.0f){
		customHue = 0.0f;
	}

	static bool isKeyDown9 = false;
	static bool isKeyDown0 = false;
	if(!isKeyDown9 && App::Key('9')){
		shldRenderTileArt = !shldRenderTileArt;
		isKeyDown9 = true;
	} else if(isKeyDown9 && !App::Key('9')){
		isKeyDown9 = false;
	}
	if(!isKeyDown0 && App::Key('0')){
		shldRenderFog = !shldRenderFog;
		isKeyDown0 = true;
	} else if(isKeyDown0 && !App::Key('0')){
		isKeyDown0 = false;
	}

	float& simSpd = sim->spd;
	static bool isKeyDownPlus = false;
	static bool isKeyDownMinus = false;
	if(!isKeyDownPlus && App::Key(VK_OEM_PLUS)){
		simSpd += 0.1f;
		isKeyDownPlus = true;
	} else if(isKeyDownPlus && !App::Key(VK_OEM_PLUS)){
		isKeyDownPlus = false;
	}
	if(!isKeyDownMinus && App::Key(VK_OEM_MINUS)){
		simSpd -= 0.1f;
		isKeyDownMinus = true;
	} else if(isKeyDownMinus && !App::Key(VK_OEM_MINUS)){
		isKeyDownMinus = false;
	}
	simSpd = Math::Clamp(simSpd, 0.2f, 4.0f);
}

void Scene::UpdateEntities(const double dt){
	const std::vector<Entity*>& entityLayer = sim->GetEntityLayer();
	const std::vector<TileType>& tileLayer = sim->GetTileLayer();
	unitsLeftPlayer = unitsLeftAI = 0;
	const int entityLayerSize = (int)entityLayer.size();

	for(int i = 0; i < entityLayerSize; ++i){
		Entity* const& entity = entityLayer[i];
		if(entity == nullptr){
			continue;
		}

		if(tileLayer[i] == TileType::Fire){
			entity->im_Attribs.im_CurrHealth -= (float)dt;
		}

		if(entity->im_Attribs.im_Team == Obj::EntityTeam::Player){
			++unitsLeftPlayer;
		} else{
			++unitsLeftAI;
		}

		entity->im_Attribs.im_TimeAlive += (float)dt;

		switch(entity->im_Attribs.im_Type){
			using Obj::EntityType;

			case EntityType::Knight:
				knightSM->CheckForStateTransition(entity);
				knightSM->UpdateCurrState(entity, dt);
				break;
			case EntityType::Gunner:
				gunnerSM->CheckForStateTransition(entity);
				gunnerSM->UpdateCurrState(entity, dt);
				break;
			case EntityType::Healer:
				healerSM->CheckForStateTransition(entity);
				healerSM->UpdateCurrState(entity, dt);
				break;
		}
	}

	if((unitsLeftPlayer + canSpawnAmtPlayer == 0) || (unitsLeftAI + canSpawnAmtAI == 0)){
		sim->status = SimRuntimeStatus::Ended;
		return;
	}

	if(entityMoving != nullptr){
		if(tileLayer[(int)entityMoving->im_Attribs.im_LocalPos.y * gridCols + (int)entityMoving->im_Attribs.im_LocalPos.x] == TileType::Fire){
			entityMoving->im_Attribs.im_CurrHealth -= (float)dt;
		}

		if(entityMoving->im_Attribs.im_Team == Obj::EntityTeam::Player){
			++unitsLeftPlayer;
		} else{
			++unitsLeftAI;
		}

		entityMoving->im_Attribs.im_TimeAlive += (float)dt;

		switch(entityMoving->im_Attribs.im_Type){
			using Obj::EntityType;

			case EntityType::Knight:
				knightSM->CheckForStateTransition(entityMoving);
				knightSM->UpdateCurrState(entityMoving, dt);
				break;
			case EntityType::Gunner:
				gunnerSM->CheckForStateTransition(entityMoving);
				gunnerSM->UpdateCurrState(entityMoving, dt);
				break;
			case EntityType::Healer:
				healerSM->CheckForStateTransition(entityMoving);
				healerSM->UpdateCurrState(entityMoving, dt);
				break;
		}
	}

	for(Entity* const entity: entitiesToDeactivate){
		sim->OnEntityDeactivated(gridCols, (int)entity->im_Attribs.im_LocalPos.y, (int)entity->im_Attribs.im_LocalPos.x);
		entityPool->DeactivateObj(entity);
	}
	entitiesToDeactivate.clear();
}

void Scene::UpdateStates(const double dt){
	UpdateKnightStates(dt);
	UpdateGunnerStates(dt);
	UpdateHealerStates(dt);
}

void Scene::UpdateKnightStates(const double dt){
}

void Scene::UpdateGunnerStates(const double dt){
}

void Scene::UpdateHealerStates(const double dt){
}

void Scene::UpdateFog(const double dt){
	std::fill(fogLayer.begin(), fogLayer.end(), true);
	static const std::vector<Entity*>& entityLayer = sim->GetEntityLayer();
	const int entityLayerSize = (int)entityLayer.size();

	if(selectedRow >= 0 && selectedCol >= 0){
		const Entity* const entity = entityLayer[selectedRow * gridCols + selectedCol];
		if(entity != nullptr
			&& ((entity->im_Attribs.im_Team == Obj::EntityTeam::Player && sim->turn == SimTurn::Player)
			|| (entity->im_Attribs.im_Team == Obj::EntityTeam::AI && sim->turn == SimTurn::AI))){
			switch(entity->im_Attribs.im_VisionType){
				case Obj::EntityVisionType::Unidirectional:
					ClearFogUnidirectional((int)entity->im_Attribs.im_LocalPos.y, (int)entity->im_Attribs.im_LocalPos.x, entity->im_Attribs.im_VisionRange, entity->im_Attribs.im_FacingDir);
					break;
				case Obj::EntityVisionType::Omnidirectional:
					ClearFogOmnidirectional((int)entity->im_Attribs.im_LocalPos.y, (int)entity->im_Attribs.im_LocalPos.x, entity->im_Attribs.im_VisionRange);
					break;
				default:
					assert(false);
			}
		}

		visionLayer = fogLayer;
	}

	for(int i = 0; i < entityLayerSize; ++i){
		const Entity* const entity = entityLayer[i];
		if(entity != nullptr
			&& ((entity->im_Attribs.im_Team == Obj::EntityTeam::Player && sim->turn == SimTurn::Player)
			|| (entity->im_Attribs.im_Team == Obj::EntityTeam::AI && sim->turn == SimTurn::AI))){
			fogLayer[i] = false;
		}
	}
}

void Scene::Render(){
	SceneSupport::Render();

	modelStack.PushMatrix();

	modelStack.Translate(
		-(float)windowWidth * 0.5f,
		-(float)windowHeight * 0.5f,
		0.0f
	);

	RenderBG();
	switch(sim->status){
		case SimRuntimeStatus::Waiting:
			RenderSimWaiting();
			break;
		case SimRuntimeStatus::MakingTheMap:
			RenderSimMakingTheMap();
			break;
		case SimRuntimeStatus::Ongoing:
			RenderSimOngoing();
			break;
		case SimRuntimeStatus::Ended:
			RenderSimEnded();
			break;
	}
	RenderSceneText();

	modelStack.PopMatrix();

	if(sim->status == SimRuntimeStatus::Waiting){
		canMakeSimMap = true;
	}
}

void Scene::RenderSimWaiting(){
	RenderCoverMap();
	RenderCoverText();
}

void Scene::RenderSimMakingTheMap(){
	RenderMap();
}

void Scene::RenderSimOngoing(){
	RenderMap();
	RenderEntities();
	if(shldRenderFog){
		glDepthFunc(GL_ALWAYS);
		RenderFog();
		glDepthFunc(GL_LESS);
	}
	RenderSelected();
	if(sim->turn == SimTurn::Player){
		RenderGridCellOfMouse();
	}
}

void Scene::RenderSimEnded(){
	RenderCoverMap();
	static bool endResult = unitsLeftPlayer + canSpawnAmtPlayer == 0;

	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod],
		endResult ? "You Lose..." : "You Win!",
		endResult ? Color(1.0f, 0.0f, 0.0f) : Color(0.0f, 1.0f, 0.0f),
		(float)windowWidth * 0.05f,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f + cosf(elapsedTime * 4.0f) * (float)windowHeight * 0.05f,
		TextAlignment::Center,
		-0.1f
	);
}

void Scene::RenderBG(){
	modelStack.PushMatrix();

	modelStack.Translate(
		im_Cam.pos.x + (float)windowWidth * 0.5f,
		im_Cam.pos.y + (float)windowHeight * 0.5f,
		0.0f
	);
	modelStack.Scale(
		(float)windowWidth,
		(float)windowHeight,
		1.0f
	);

	switch(sim->timeOfDay){
		case SimTimeOfDay::Day:
			RenderMesh(meshList[(int)GeoType::DayBG], false);
			break;
		case SimTimeOfDay::Rainy:
			RenderMesh(meshList[(int)GeoType::RainyBG], false);
			break;
		case SimTimeOfDay::Night:
			RenderMesh(meshList[(int)GeoType::NightBG], false);
			break;
	}

	modelStack.PopMatrix();
}

void Scene::RenderCoverMap(){
	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			modelStack.PushMatrix();

			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Translate(
					gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * (float)c,
					gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * (float)r + (c & 1) * gridAltOffsetY,
					0.05f
				);
				modelStack.Scale(
					gridCellScaleX + gridLineThickness * 2.5f,
					gridCellScaleY + gridLineThickness * 2.5f,
					1.0f
				);
			} else{
				modelStack.Translate(
					gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * (float)c + (r & 1) * gridAltOffsetX,
					gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * (float)r,
					0.05f
				);
				modelStack.Rotate(
					90.0f,
					0.0f,
					0.0f,
					1.0f
				);
				modelStack.Scale(
					gridCellScaleY + gridLineThickness * 2.5f,
					gridCellScaleX + gridLineThickness * 2.5f,
					1.0f
				);
			}

			RenderMesh(meshList[(int)GeoType::Hex], true, Color::HSVToRGB({customHue, 1.0f, 1.0f}), 0.5f);

			modelStack.PopMatrix();

			modelStack.PushMatrix();

			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Translate(
					gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * (float)c,
					gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * (float)r + (c & 1) * gridAltOffsetY,
					0.1f
				);
				modelStack.Scale(
					gridCellScaleX,
					gridCellScaleY,
					1.0f
				);
			} else{
				modelStack.Translate(
					gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * (float)c + (r & 1) * gridAltOffsetX,
					gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * (float)r,
					0.1f
				);
				modelStack.Rotate(
					90.0f,
					0.0f,
					0.0f,
					1.0f
				);
				modelStack.Scale(
					gridCellScaleY,
					gridCellScaleX,
					1.0f
				);
			}

			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 0.0f, 0.0f), 0.5f);

			modelStack.PopMatrix();
		}
	}
}

void Scene::RenderCoverText(){
	static float textSize0 = (float)windowWidth * 0.025f;

	static float startTextSize = (float)windowWidth * 0.02f;
	static float endTextSize = (float)windowWidth * 0.03f;

	const float lerpFactor1 = EaseInQuint(cosf(elapsedTime * 7.0f) * 0.5f + 0.5f);
	const float textSize1 = (1.0f - lerpFactor1) * startTextSize + lerpFactor1 * endTextSize;

	const float lerpFactor2 = EaseInQuint(sinf(elapsedTime * 7.0f) * 0.5f + 0.5f);
	const float textSize2 = (1.0f - lerpFactor2) * startTextSize + lerpFactor2 * endTextSize;

	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod],
		"Press Middle Mouse Button (MMB) to start!",
		Color(),
		textSize1,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f + textSize0 + textSize1,
		TextAlignment::Center,
		-0.1f
	);
	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod],
		"Modify hex grid (1 - 8, ~)",
		Color(),
		textSize0,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f,
		TextAlignment::Center,
		-0.1f
	);
	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod],
		"Press Middle Mouse Button (MMB) to start!",
		Color(),
		textSize2,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f - textSize0 - textSize2,
		TextAlignment::Center,
		-0.1f
	);
}

void Scene::RenderEntities(){
	static float individualDepthOffset = 0.0f;
	const std::vector<Entity*>& entityLayer = sim->GetEntityLayer();

	for(const Entity* const entity: entityLayer){
		if(entity == nullptr){
			continue;
		}

		modelStack.PushMatrix();
		const Vector3 localPos = Vector3(entity->im_Attribs.im_LocalPos.x, entity->im_Attribs.im_LocalPos.y, 0.0f);

		if(gridType == HexGrid<float>::GridType::FlatTop){
			modelStack.Translate(
				gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * localPos.x,
				gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * localPos.y + ((int)localPos.x & 1) * gridAltOffsetY,
				0.25f + individualDepthOffset
			);
			modelStack.Scale(
				gridCellScaleX * 0.8f,
				gridCellScaleY * 0.8f,
				1.0f
			);
		} else{
			modelStack.Translate(
				gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * localPos.x + ((int)localPos.y & 1) * gridAltOffsetX,
				gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * localPos.y,
				0.25f + individualDepthOffset
			);
			modelStack.Scale(
				gridCellScaleY * 0.8f,
				gridCellScaleX * 0.8f,
				1.0f
			);
		}

		RenderMesh(
			meshList[(int)GeoType::Circle],
			true,
			entity->im_Attribs.im_Team == Obj::EntityTeam::AI ? Color(0.4f, 0.0f, 0.0f) : Color(0.0f, 0.4f, 0.0f),
			entity->im_Attribs.im_DeadMaxTime > 0.0f ? entity->im_Attribs.im_DeadCurrTime / entity->im_Attribs.im_DeadMaxTime : 1.0f
		);
		RenderEntityArt(entity);
		RenderHealthBar(entity);
		RenderEntityState(entity);

		modelStack.PopMatrix();

		individualDepthOffset += 0.01f;
	}

	if(entityMoving != nullptr){
		modelStack.PushMatrix();
		const Vector3 localPos = Vector3(entityMoving->im_Attribs.im_LocalPos.x, entityMoving->im_Attribs.im_LocalPos.y, 0.0f);
		static float offset = 0.0f;

		if(gridType == HexGrid<float>::GridType::FlatTop){
			if(localPos.x - entityMoving->im_Attribs.im_GridCellTargetLocalPos.x <= Math::EPSILON
				&& entityMoving->im_Attribs.im_GridCellTargetLocalPos.x - localPos.x <= Math::EPSILON){
				offset = ((int)localPos.x & 1) * gridAltOffsetY;
			} else{
				offset = (int(entityMoving->im_Attribs.im_GridCellTargetLocalPos.x > entityMoving->im_Attribs.im_GridCellStartLocalPos.x
					? entityMoving->im_Attribs.im_GridCellTargetLocalPos.x
					: entityMoving->im_Attribs.im_GridCellStartLocalPos.x) & 1
					? localPos.x - std::floorf(localPos.x)
					: -(localPos.x - std::ceilf(localPos.x)))
					* gridAltOffsetY;
			}

			modelStack.Translate(
				gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * localPos.x,
				gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * localPos.y + offset,
				0.25f + individualDepthOffset
			);
			modelStack.Scale(
				gridCellScaleX * 0.8f,
				gridCellScaleY * 0.8f,
				1.0f
			);
		} else{
			if(localPos.y - entityMoving->im_Attribs.im_GridCellTargetLocalPos.y <= Math::EPSILON
				&& entityMoving->im_Attribs.im_GridCellTargetLocalPos.y - localPos.y <= Math::EPSILON){
				offset = ((int)localPos.y & 1) * gridAltOffsetX;
			} else{
				offset = (int(entityMoving->im_Attribs.im_GridCellTargetLocalPos.y > entityMoving->im_Attribs.im_GridCellStartLocalPos.y
					? entityMoving->im_Attribs.im_GridCellTargetLocalPos.y
					: entityMoving->im_Attribs.im_GridCellStartLocalPos.y) & 1
					? localPos.y - std::floorf(localPos.y)
					: -(localPos.y - std::ceilf(localPos.y)))
					* gridAltOffsetX;
			}

			modelStack.Translate(
				gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * localPos.x + offset,
				gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * localPos.y,
				0.25f + individualDepthOffset
			);
			modelStack.Scale(
				gridCellScaleY * 0.8f,
				gridCellScaleX * 0.8f,
				1.0f
			);
		}

		RenderMesh(
			meshList[(int)GeoType::Circle],
			true,
			entityMoving->im_Attribs.im_Team == Obj::EntityTeam::AI ? Color(0.4f, 0.0f, 0.0f) : Color(0.0f, 0.4f, 0.0f),
			entityMoving->im_Attribs.im_DeadMaxTime > 0.0f ? entityMoving->im_Attribs.im_DeadCurrTime / entityMoving->im_Attribs.im_DeadMaxTime : 1.0f
		);
		RenderEntityArtMoving(entityMoving);
		RenderHealthBar(entityMoving);
		RenderEntityState(entityMoving);

		modelStack.PopMatrix();
	}

	individualDepthOffset = 0.0f;
}

void Scene::RenderEntityArt(const Entity* const entity){
	modelStack.PushMatrix();

	modelStack.Translate(
		0.0f,
		0.0f,
		0.05f
	);

	if(gridType == HexGrid<float>::GridType::FlatTop){
		if(((int)entity->im_Attribs.im_LocalPos.x & 1) == 1){
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					//Do nth
					break;
				case Obj::EntityFacingDir::Down:
					modelStack.Rotate(
						180.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Left:
					modelStack.Rotate(
						120.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Right:
					modelStack.Rotate(
						240.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::UL:
					modelStack.Rotate(
						60.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::UR:
					modelStack.Rotate(
						300.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
			}
		} else{
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					//Do nth
					break;
				case Obj::EntityFacingDir::Down:
					modelStack.Rotate(
						180.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Left:
					modelStack.Rotate(
						60.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Right:
					modelStack.Rotate(
						300.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::DL:
					modelStack.Rotate(
						120.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::DR:
					modelStack.Rotate(
						240.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
			}
		}
	} else{
		if(((int)entity->im_Attribs.im_LocalPos.y & 1) == 1){
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					modelStack.Rotate(
						30.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Down:
					modelStack.Rotate(
						150.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Left:
					modelStack.Rotate(
						90.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Right:
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::DR:
					modelStack.Rotate(
						210.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::UR:
					modelStack.Rotate(
						330.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
			}
		} else{
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					modelStack.Rotate(
						330.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Down:
					modelStack.Rotate(
						210.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Left:
					modelStack.Rotate(
						90.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Right:
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::DL:
					modelStack.Rotate(
						150.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::UL:
					modelStack.Rotate(
						30.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
			}
		}
	}

	modelStack.Scale(
		0.6f,
		0.6f,
		1.0f
	);

	switch(entity->im_Attribs.im_Type){
		case Obj::EntityType::Knight:
			RenderMesh(
				meshList[(int)GeoType::Knight],
				true,
				Color(),
				entity->im_Attribs.im_DeadMaxTime > 0.0f ? entity->im_Attribs.im_DeadCurrTime / entity->im_Attribs.im_DeadMaxTime : 1.0f
			);
			break;
		case Obj::EntityType::Gunner:
			RenderMesh(
				meshList[(int)GeoType::Gunner],
				true,
				Color(),
				entity->im_Attribs.im_DeadMaxTime > 0.0f ? entity->im_Attribs.im_DeadCurrTime / entity->im_Attribs.im_DeadMaxTime : 1.0f
			);
			break;
		case Obj::EntityType::Healer:
			RenderMesh(
				meshList[(int)GeoType::Healer],
				true,
				Color(),
				entity->im_Attribs.im_DeadMaxTime > 0.0f ? entity->im_Attribs.im_DeadCurrTime / entity->im_Attribs.im_DeadMaxTime : 1.0f
			);
			break;
	}

	modelStack.PopMatrix();
}

void Scene::RenderEntityArtMoving(const Entity* const entity){
	modelStack.PushMatrix();

	modelStack.Translate(
		0.0f,
		0.0f,
		0.05f
	);

	if(gridType == HexGrid<float>::GridType::FlatTop){
		if(((int)entity->im_Attribs.im_GridCellStartLocalPos.x & 1) == 1){
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					//Do nth
					break;
				case Obj::EntityFacingDir::Down:
					modelStack.Rotate(
						180.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Left:
					modelStack.Rotate(
						120.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Right:
					modelStack.Rotate(
						240.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::UL:
					modelStack.Rotate(
						60.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::UR:
					modelStack.Rotate(
						300.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
			}
		} else{
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					//Do nth
					break;
				case Obj::EntityFacingDir::Down:
					modelStack.Rotate(
						180.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Left:
					modelStack.Rotate(
						60.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Right:
					modelStack.Rotate(
						300.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::DL:
					modelStack.Rotate(
						120.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::DR:
					modelStack.Rotate(
						240.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
			}
		}
	} else{
		if(((int)entity->im_Attribs.im_GridCellStartLocalPos.y & 1) == 1){
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					modelStack.Rotate(
						30.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Down:
					modelStack.Rotate(
						150.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Left:
					modelStack.Rotate(
						90.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Right:
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::DR:
					modelStack.Rotate(
						210.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::UR:
					modelStack.Rotate(
						330.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
			}
		} else{
			switch(entity->im_Attribs.im_FacingDir){
				case Obj::EntityFacingDir::Up:
					modelStack.Rotate(
						330.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Down:
					modelStack.Rotate(
						210.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Left:
					modelStack.Rotate(
						90.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::Right:
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::DL:
					modelStack.Rotate(
						150.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
				case Obj::EntityFacingDir::UL:
					modelStack.Rotate(
						30.0f,
						0.0f,
						0.0f,
						1.0f
					);
					break;
			}
		}
	}

	modelStack.Scale(
		0.6f,
		0.6f,
		1.0f
	);

	switch(entity->im_Attribs.im_Type){
		case Obj::EntityType::Knight:
			RenderMesh(
				meshList[(int)GeoType::Knight],
				true,
				Color(),
				entity->im_Attribs.im_DeadMaxTime > 0.0f ? entity->im_Attribs.im_DeadCurrTime / entity->im_Attribs.im_DeadMaxTime : 1.0f
			);
			break;
		case Obj::EntityType::Gunner:
			RenderMesh(
				meshList[(int)GeoType::Gunner],
				true,
				Color(),
				entity->im_Attribs.im_DeadMaxTime > 0.0f ? entity->im_Attribs.im_DeadCurrTime / entity->im_Attribs.im_DeadMaxTime : 1.0f
			);
			break;
		case Obj::EntityType::Healer:
			RenderMesh(
				meshList[(int)GeoType::Healer],
				true,
				Color(),
				entity->im_Attribs.im_DeadMaxTime > 0.0f ? entity->im_Attribs.im_DeadCurrTime / entity->im_Attribs.im_DeadMaxTime : 1.0f
			);
			break;
	}

	modelStack.PopMatrix();
}

void Scene::RenderHealthBar(const Entity* const entity){
	if(entity->im_Attribs.im_CurrHealth <= 0.0f){
		return;
	}

	const float ratio = entity->im_Attribs.im_CurrHealth / entity->im_Attribs.im_MaxHealth;
	modelStack.PushMatrix();

	modelStack.Translate(
		0.0f,
		0.3f,
		0.1f
	);
	modelStack.Scale(
		0.5f,
		0.07f,
		1.0f
	);

	modelStack.PushMatrix();

	modelStack.Translate(
		-(1.0f - ratio) * 0.5f,
		0.0f,
		0.0f
	);
	modelStack.Scale(
		ratio - 0.04f,
		0.8f,
		1.0f
	);

	RenderMesh(meshList[(int)GeoType::Quad], true, Color::HSVToRGB({ratio * 120.0f, 1.0f, 1.0f}), 1.0f);

	modelStack.PopMatrix();

	RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.1f, 0.1f, 0.1f), 1.0f);

	modelStack.PopMatrix();
}

void Scene::RenderEntityState(const Entity* const entity){
	modelStack.PushMatrix();

	modelStack.Translate(
		0.0f,
		-0.4f,
		0.1f
	);
	modelStack.Scale(
		0.22f,
		0.22f,
		1.0f
	);

	static std::string stateText = std::string();
	switch(entity->im_Attribs.im_CurrState->GetID()){
		case StateID::StateIdleKnight:
			stateText = "KnightIdle";
			break;
		case StateID::StateIdleGunner:
			stateText = "GunnerIdle";
			break;
		case StateID::StateIdleHealer:
			stateText = "HealerIdle";
			break;
		case StateID::StatePatrolKnight:
			stateText = "KnightPatrol";
			break;
		case StateID::StatePatrolGunner:
			stateText = "GunnerPatrol";
			break;
		case StateID::StateChaseKnight:
			stateText = "KnightChase";
			break;
		case StateID::StateChaseGunner:
			stateText = "GunnerChase";
			break;
		case StateID::StateChaseHealer:
			stateText = "HealerChase";
			break;
		case StateID::StateAttackKnight:
			stateText = "KnightAttack";
			break;
		case StateID::StateAttackGunner:
			stateText = "GunnerAttack";
			break;
		case StateID::StateHealHealer:
			stateText = "HealerHeal";
			break;
		case StateID::StateDeadKnight:
			stateText = "KnightDead";
			break;
		case StateID::StateDeadGunner:
			stateText = "GunnerDead";
			break;
		case StateID::StateDeadHealer:
			stateText = "HealerDead";
			break;
	}

	RenderText(
		meshList[(int)GeoType::TextMod],
		stateText,
		Color(),
		TextAlignment::Center,
		-0.1f
	);

	modelStack.PopMatrix();
}

void Scene::RenderMap(){
	const std::vector<TileType>& tileLayer = sim->GetTileLayer();
	const int tileLayerSize = (int)tileLayer.size();

	static Color turnColor = Color();
	switch(sim->turn){
		case SimTurn::AI:
			turnColor = Color(0.4f, 0.0f, 0.0f);
			break;
		case SimTurn::Environment:
			turnColor = Color(0.0f, 0.0f, 0.0f);
			break;
		case SimTurn::Player:
			turnColor = Color(0.0f, 0.4f, 0.0f);
			break;
	}

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			modelStack.PushMatrix();

			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Translate(
					gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * (float)c,
					gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * (float)r + (c & 1) * gridAltOffsetY,
					0.05f
				);
				modelStack.Scale(
					gridCellScaleX + gridLineThickness * 2.5f,
					gridCellScaleY + gridLineThickness * 2.5f,
					1.0f
				);
			} else{
				modelStack.Translate(
					gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * (float)c + (r & 1) * gridAltOffsetX,
					gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * (float)r,
					0.05f
				);
				modelStack.Rotate(
					90.0f,
					0.0f,
					0.0f,
					1.0f
				);
				modelStack.Scale(
					gridCellScaleY + gridLineThickness * 2.5f,
					gridCellScaleX + gridLineThickness * 2.5f,
					1.0f
				);
			}

			RenderMesh(meshList[(int)GeoType::Hex], true, turnColor, 0.7f);

			modelStack.PopMatrix();

			if(r * gridCols + c >= tileLayerSize){
				continue;
			}

			modelStack.PushMatrix();

			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Translate(
					gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * (float)c,
					gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * (float)r + (c & 1) * gridAltOffsetY,
					0.1f
				);
				modelStack.Scale(
					gridCellScaleX,
					gridCellScaleY,
					1.0f
				);
			} else{
				modelStack.Translate(
					gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * (float)c + (r & 1) * gridAltOffsetX,
					gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * (float)r,
					0.1f
				);
				modelStack.Rotate(
					90.0f,
					0.0f,
					0.0f,
					1.0f
				);
				modelStack.Scale(
					gridCellScaleY,
					gridCellScaleX,
					1.0f
				);
			}

			RenderTile(tileLayer, r, c);

			modelStack.PopMatrix();
		}
	}
}

void Scene::RenderFog(){
	const float offsetX = sinf(10.0f * elapsedTime);
	const float offsetY = cosf(10.0f * elapsedTime);

	static float startAngle = -400.0f;
	static float endAngle = 400.0f;
	const float lerpFactor = EaseInOutSine((sinf(0.4f * elapsedTime) + cosf(0.4f * elapsedTime)) * 0.25f + 0.5f);
	const float angle = (1.0f - lerpFactor) * startAngle + lerpFactor * endAngle;

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			if(!fogLayer[r * gridCols + c]){
				continue;
			}

			modelStack.PushMatrix();

			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Translate(
					gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * (float)c + offsetX,
					gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * (float)r + (c & 1) * gridAltOffsetY + offsetY,
					0.1f
				);
				modelStack.Rotate(
					angle,
					0.0f,
					0.0f,
					1.0f
				);
				modelStack.Scale(
					gridCellScaleX,
					gridCellScaleY,
					1.0f
				);
			} else{
				modelStack.Translate(
					gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * (float)c + (r & 1) * gridAltOffsetX + offsetX,
					gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * (float)r + offsetY,
					0.1f
				);
				modelStack.Rotate(
					angle,
					0.0f,
					0.0f,
					1.0f
				);
				modelStack.Scale(
					gridCellScaleY,
					gridCellScaleX,
					1.0f
				);
			}

			RenderMesh(meshList[(int)GeoType::Fog], true, Color(1.0f, 0.63f, 0.63f), 0.8f);

			modelStack.PopMatrix();
		}
	}
}

void Scene::RenderTile(const std::vector<TileType>& tileLayer, const int r, const int c){
	switch(tileLayer[r * gridCols + c]){
		case TileType::Empty:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.7f, 0.7f, 0.7f), 1.0f);
			break;
		case TileType::Wall:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color::HSVToRGB({customHue, 1.0f, 1.0f}), 1.0f);

			if(shldRenderTileArt){
				modelStack.PushMatrix();

				modelStack.Translate(
					0.0f,
					0.0f,
					0.05f
				);

				RenderMesh(meshList[(int)GeoType::WallTile], true, Color(0.6f, 0.6f, 0.6f), 1.0f);

				modelStack.PopMatrix();
			}
			break;
		case TileType::Soil:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.7f, 0.4f, 0.11f), 1.0f);

			if(shldRenderTileArt){
				modelStack.PushMatrix();

				modelStack.Translate(
					0.0f,
					0.0f,
					0.05f
				);
				if(gridType == HexGrid<float>::GridType::SharpTop){
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
				modelStack.Scale(
					0.6f,
					0.6f,
					1.0f
				);

				RenderMesh(meshList[(int)GeoType::SoilTile], false);

				modelStack.PopMatrix();
			}
			break;
		case TileType::Fire:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.73f, 0.0f, 0.0f), 1.0f);

			if(shldRenderTileArt){
				modelStack.PushMatrix();

				modelStack.Translate(
					0.0f,
					0.0f,
					0.05f
				);
				if(gridType == HexGrid<float>::GridType::SharpTop){
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
				modelStack.Scale(
					0.6f,
					0.6f,
					1.0f
				);

				RenderMesh(meshList[(int)GeoType::FireTile], true, Color(0.9f, 0.9f, 0.9f), 1.0f);

				modelStack.PopMatrix();
			}
			break;
		case TileType::Water:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.05f, 0.64f, 1.0f), 1.0f);

			if(shldRenderTileArt){
				modelStack.PushMatrix();

				modelStack.Translate(
					0.0f,
					0.0f,
					0.05f
				);
				if(gridType == HexGrid<float>::GridType::SharpTop){
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
				modelStack.Scale(
					0.7f,
					0.5f,
					1.0f
				);

				RenderMesh(meshList[(int)GeoType::WaterTile], false);

				modelStack.PopMatrix();
			}
			break;
		case TileType::Grass:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.49f, 0.78f, 0.31f), 1.0f);

			if(shldRenderTileArt){
				modelStack.PushMatrix();

				modelStack.Translate(
					0.0f,
					0.0f,
					0.05f
				);
				if(gridType == HexGrid<float>::GridType::SharpTop){
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
				modelStack.Scale(
					0.6f,
					0.6f,
					1.0f
				);

				RenderMesh(meshList[(int)GeoType::GrassTile], true, Color(0.7f, 0.7f, 0.7f), 1.0f);

				modelStack.PopMatrix();
			}
			break;
		case TileType::Mud:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.4f, 0.2f, 0.1f), 1.0f);

			if(shldRenderTileArt){
				modelStack.PushMatrix();

				modelStack.Translate(
					0.0f,
					0.0f,
					0.05f
				);
				if(gridType == HexGrid<float>::GridType::SharpTop){
					modelStack.Rotate(
						270.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
				modelStack.Scale(
					0.6f,
					0.6f,
					1.0f
				);

				RenderMesh(meshList[(int)GeoType::MudTile], false);

				modelStack.PopMatrix();
			}
			break;
	}
}

void Scene::RenderGridCellOfMouse(){
	if(mouseRow >= 0 && mouseRow <= gridRows - 1 && mouseCol >= 0 && mouseCol <= gridCols - 1){
		modelStack.PushMatrix();

		if(gridType == HexGrid<float>::GridType::FlatTop){
			modelStack.Translate(
				gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * mouseCol,
				gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * mouseRow + ((int)mouseCol & 1) * gridAltOffsetY,
				0.99f
			);
			modelStack.Scale(
				gridCellScaleX + gridLineThickness * 2.5f,
				gridCellScaleY + gridLineThickness * 2.5f,
				1.0f
			);
		} else{
			modelStack.Translate(
				gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * mouseCol + ((int)mouseRow & 1) * gridAltOffsetX,
				gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * mouseRow,
				0.99f
			);
			modelStack.Rotate(
				90.0f,
				0.0f,
				0.0f,
				1.0f
			);
			modelStack.Scale(
				gridCellScaleY + gridLineThickness * 2.5f,
				gridCellScaleX + gridLineThickness * 2.5f,
				1.0f
			);
		}

		RenderMesh(meshList[(int)GeoType::Hex], true, Color(), 0.4f);

		modelStack.PopMatrix();
	}
}

void Scene::RenderSelected(){
	if(selectedRow >= 0 && selectedCol >= 0){
		modelStack.PushMatrix();

		if(gridType == HexGrid<float>::GridType::FlatTop){
			modelStack.Translate(
				gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * (float)selectedCol,
				gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * (float)selectedRow + (selectedCol & 1) * gridAltOffsetY,
				1.0f
			);
			modelStack.Scale(
				gridCellScaleX + gridLineThickness * 2.5f,
				gridCellScaleY + gridLineThickness * 2.5f,
				1.0f
			);
		} else{
			modelStack.Translate(
				gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * (float)selectedCol + (selectedRow & 1) * gridAltOffsetX,
				gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * (float)selectedRow,
				1.0f
			);
			modelStack.Rotate(
				90.0f,
				0.0f,
				0.0f,
				1.0f
			);
			modelStack.Scale(
				gridCellScaleY + gridLineThickness * 2.5f,
				gridCellScaleX + gridLineThickness * 2.5f,
				1.0f
			);
		}

		RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 1.0f, 1.0f), 0.4f);

		modelStack.PopMatrix();
	}

	if(selectedTargetRow >= 0 && selectedTargetCol >= 0){
		modelStack.PushMatrix();

		if(gridType == HexGrid<float>::GridType::FlatTop){
			modelStack.Translate(
				gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * (float)selectedTargetCol,
				gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * (float)selectedTargetRow + (selectedTargetCol & 1) * gridAltOffsetY,
				1.0f
			);
			modelStack.Scale(
				gridCellScaleX + gridLineThickness * 2.5f,
				gridCellScaleY + gridLineThickness * 2.5f,
				1.0f
			);
		} else{
			modelStack.Translate(
				gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * (float)selectedTargetCol + (selectedTargetRow & 1) * gridAltOffsetX,
				gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * (float)selectedTargetRow,
				1.0f
			);
			modelStack.Rotate(
				90.0f,
				0.0f,
				0.0f,
				1.0f
			);
			modelStack.Scale(
				gridCellScaleY + gridLineThickness * 2.5f,
				gridCellScaleX + gridLineThickness * 2.5f,
				1.0f
			);
		}

		RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 0.0f, 1.0f), 0.4f);

		modelStack.PopMatrix();
	}
}

void Scene::RenderSceneText(){
	static Mesh* const textMesh = meshList[(int)GeoType::TextMod];
	static float textSize = (float)windowWidth * 0.02f;

	RenderTextOnScreen(
		textMesh,
		"AI's Credits: " + std::to_string(creditsAI),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		(float)windowHeight - textSize,
		TextAlignment::Center,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"canSpawnAmtAI: " + std::to_string(canSpawnAmtAI),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		(float)windowHeight - textSize * 2.0f,
		TextAlignment::Center,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"unitsLeftAI: " + std::to_string(unitsLeftAI),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		(float)windowHeight - textSize * 3.0f,
		TextAlignment::Center,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"Your Credits: " + std::to_string(creditsPlayer),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		0.0f,
		TextAlignment::Center,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"canSpawnAmtPlayer: " + std::to_string(canSpawnAmtPlayer),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		textSize,
		TextAlignment::Center,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"unitsLeftPlayer: " + std::to_string(unitsLeftPlayer),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		textSize * 2.0f,
		TextAlignment::Center,
		-0.1f
	);

	RenderDebugInfoText(textMesh, Color(0.0f, 1.0f, 0.0f), textSize);
	RenderControlsText(textMesh, Color(1.0f, 0.0f, 1.0f), textSize);
	RenderGridInfoText(textMesh, Color(1.0f, 1.0f, 0.0f), textSize);
	RenderSimInfoText(textMesh, Color(1.0f, 0.5f, 0.0f), textSize);
}

void Scene::RenderDebugInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
	RenderTextOnScreen(
		textMesh,
		"Elapsed time: " + std::to_string(elapsedTime).substr(0, std::to_string((int)elapsedTime).length() + 3),
		textColor,
		textSize,
		0.0f,
		textSize * 0.0f,
		TextAlignment::Left,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"Update FPS: " + std::to_string(updateFPS).substr(0, std::to_string((int)updateFPS).length() + 3),
		textColor,
		textSize,
		0.0f,
		textSize * 1.0f,
		TextAlignment::Left,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"Render FPS: " + std::to_string(renderFPS).substr(0, std::to_string((int)renderFPS).length() + 3),
		textColor,
		textSize,
		0.0f,
		textSize * 2.0f,
		TextAlignment::Left,
		-0.1f
	);
}

void Scene::RenderControlsText(Mesh* const textMesh, const Color& textColor, const float textSize){
	static std::string texts[]{
		"F1: Toggle fullscreen",
		"F2: Change polygon mode",
		"F3: Toggle visibility of app window",
		"[: Slow down making of sim map",
		"]: Spd up making of sim map",
		"Backslash: Finish curr step in making...",
		"WASD: Move cam",
		"R: Reset cam",
		"9: Toggle tile art",
		"0: Toggle fog",
		"+: Increase sim spd",
		"-: Decrease sim spd",
		"Space: End your turn",
	};
	static size_t size = sizeof(texts) / sizeof(texts[0]);

	for(size_t i = 0; i < size; ++i){
		RenderTextOnScreen(
			textMesh,
			texts[i],
			textColor,
			textSize,
			0.0f,
			(float)windowHeight - textSize * float(i + 1),
			TextAlignment::Left,
			-0.1f
		);
	}
}

void Scene::RenderGridInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
	RenderTextOnScreen(
		textMesh,
		"Grid cell scale x: " + std::to_string(gridCellScaleX).substr(0, std::to_string((int)gridCellScaleX).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 6.0f,
		TextAlignment::Right,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"Grid cell scale y: " + std::to_string(gridCellScaleY).substr(0, std::to_string((int)gridCellScaleY).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 5.0f,
		TextAlignment::Right,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"Grid line thickness: " + std::to_string(gridLineThickness).substr(0, std::to_string((int)gridLineThickness).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 4.0f,
		TextAlignment::Right,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"Grid rows: " + std::to_string(gridRows),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 3.0f,
		TextAlignment::Right,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		"Grid cols: " + std::to_string(gridCols),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 2.0f,
		TextAlignment::Right,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		(std::string)"shldRenderTileArt: " + (shldRenderTileArt ? "Yes" : "No"),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 1.0f,
		TextAlignment::Right,
		-0.1f
	);
	RenderTextOnScreen(
		textMesh,
		(std::string)"shldRenderFog: " + (shldRenderFog ? "Yes" : "No"),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 0.0f,
		TextAlignment::Right,
		-0.1f
	);
}

void Scene::RenderSimInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
	static std::string runtimeStatusTexts[(int)SimRuntimeStatus::Amt]{
		"is waiting",
		"is making the map",
		"is ongoing",
		"has ended",
	};
	static std::string turnTexts[(int)SimTurn::Amt]{
		"Nil",
		"Player",
		"AI",
		"Environment"
	};

	const std::string texts[]{
		(std::string)"Sim " + runtimeStatusTexts[(int)sim->status],
		"Sim spd: " + std::to_string(sim->spd).substr(0, std::to_string((int)sim->spd).length() + 3),
		"Sim time of day duration: " + std::to_string(sim->timeOfDayDuration).substr(0, std::to_string((int)sim->timeOfDayDuration).length() + 3),
		"Sim turn duration (AI): " + std::to_string(sim->turnDurationAI).substr(0, std::to_string((int)sim->turnDurationAI).length() + 3),
		"Sim turn duration (Environment): " + std::to_string(sim->turnDurationEnvironment).substr(0, std::to_string((int)sim->turnDurationEnvironment).length() + 3),
		"Sim turn duration (Player): " + std::to_string(sim->turnDurationPlayer).substr(0, std::to_string((int)sim->turnDurationPlayer).length() + 3),
		"Sim time of day elapsed time: " + std::to_string(sim->timeOfDayElapsedTime).substr(0, std::to_string((int)sim->timeOfDayElapsedTime).length() + 3),
		"Sim turn elapsed time: " + std::to_string(sim->turnElapsedTime).substr(0, std::to_string((int)sim->turnElapsedTime).length() + 3),
		"Sim turn: " + turnTexts[(int)sim->turn],
	};
	const size_t size = sizeof(texts) / sizeof(texts[0]);

	for(size_t i = 0; i < size; ++i){
		RenderTextOnScreen(
			textMesh,
			texts[i],
			textColor,
			textSize,
			(float)windowWidth,
			(float)windowHeight - textSize * float(i + 1),
			TextAlignment::Right,
			-0.1f
		);
	}
}

void Scene::MakeSimMap(){
	sim->status = SimRuntimeStatus::MakingTheMap;

	sim->ChangeTileWeight((int)TileType::Invalid, 0);
	sim->ChangeTileWeight((int)TileType::Wall, 70);
	sim->ChangeTileWeight((int)TileType::Empty, 10);
	sim->ChangeTileWeight((int)TileType::Soil, 30);
	sim->ChangeTileWeight((int)TileType::Fire, 20);
	sim->ChangeTileWeight((int)TileType::Water, 20);
	sim->ChangeTileWeight((int)TileType::Grass, 20);
	sim->ChangeTileWeight((int)TileType::Mud, 20);

	const bool isFlatTop = gridType == HexGrid<float>::GridType::FlatTop;
	const float quickRenderDelay0 = 0.05f;
	const float quickRenderDelay1 = 0.02f;
	const float quickRenderDelay2 = 0.04f;

	Math::InitRNG();
	sim->GenTileLayer(gridRows, gridCols, 0, 0, Math::RandIntMinMax(0, 4040), &quickRenderDelay0, isFlatTop);
	sim->RefineTileLayer(gridRows, gridCols, Math::RandIntMinMax(0, 4000), &quickRenderDelay1);
	
	const int amtOfHoles = Math::RandIntMinMax(1, 4);
	if(amtOfHoles == 1){
		sim->MakeRadialHoleInTileLayer(
			gridRows,
			gridCols,
			Math::RandIntMinMax(gridRows / 2 - 3, gridRows / 2 + 3),
			Math::RandIntMinMax(gridCols / 2 - 3, gridCols / 2 + 3),
			3,
			&quickRenderDelay2,
			isFlatTop
		);
	} else{
		for(int i = 0; i < amtOfHoles; ++i){
			sim->MakeRadialHoleInTileLayer(
				gridRows,
				gridCols,
				Math::RandIntMinMax(0, gridRows - 1),
				Math::RandIntMinMax(0, gridCols - 1),
				Math::RandIntMinMax(0, 2),
				&quickRenderDelay2,
				isFlatTop
			);
		}
	}

	sim->status = SimRuntimeStatus::Ongoing;

	if(sim->turn == SimTurn::Player){
		creditsPlayer += 100;
	} else if(sim->turn == SimTurn::AI){
		creditsAI += 100;
	}
}

void Scene::ClearFogUnidirectional(const int row, const int col, const int range, const Obj::EntityFacingDir facingDir){
	std::vector<std::pair<int, int>> myVec;
	myVec.emplace_back(std::make_pair(0, row * gridCols + col));

	while(!myVec.empty()){
		const std::pair<int, int> myPair = myVec.front();
		if(!fogLayer[myPair.second]){
			myVec.erase(myVec.begin());
			continue;
		}
		fogLayer[myPair.second] = false;

		if(myPair.first != range){
			const int myCol = myPair.second % gridCols;
			const int myRow = myPair.second / gridCols;

			Obj::EntityFacingDir facingDirProxy = Obj::EntityFacingDir::Invalid;

			if(gridType == HexGrid<float>::GridType::FlatTop){
				if(((col & 1) && (myCol & 1)) || ((~col & 1) && (~myCol & 1))){
					facingDirProxy = facingDir;
				} else{
					switch(facingDir){
						case Obj::EntityFacingDir::Left:
							if((col & 1) == 1){
								facingDirProxy = Obj::EntityFacingDir::DL;
							} else{
								facingDirProxy = Obj::EntityFacingDir::UL;
							}
							break;
						case Obj::EntityFacingDir::Right:
							if((col & 1) == 1){
								facingDirProxy = Obj::EntityFacingDir::DR;
							} else{
								facingDirProxy = Obj::EntityFacingDir::UR;
							}
							break;
						case Obj::EntityFacingDir::UL:
						case Obj::EntityFacingDir::DL:
							facingDirProxy = Obj::EntityFacingDir::Left;
							break;
						case Obj::EntityFacingDir::UR:
						case Obj::EntityFacingDir::DR:
							facingDirProxy = Obj::EntityFacingDir::Right;
							break;
						default:
							facingDirProxy = facingDir;
					}
				}
			} else{
				if(((row & 1) && (myRow & 1)) || ((~row & 1) && (~myRow & 1))){
					facingDirProxy = facingDir;
				} else{
					switch(facingDir){
						case Obj::EntityFacingDir::Up:
							if((row & 1) == 1){
								facingDirProxy = Obj::EntityFacingDir::UL;
							} else{
								facingDirProxy = Obj::EntityFacingDir::UR;
							}
							break;
						case Obj::EntityFacingDir::Down:
							if((row & 1) == 1){
								facingDirProxy = Obj::EntityFacingDir::DL;
							} else{
								facingDirProxy = Obj::EntityFacingDir::DR;
							}
							break;
						case Obj::EntityFacingDir::UL:
						case Obj::EntityFacingDir::UR:
							facingDirProxy = Obj::EntityFacingDir::Up;
							break;
						case Obj::EntityFacingDir::DL:
						case Obj::EntityFacingDir::DR:
							facingDirProxy = Obj::EntityFacingDir::Down;
							break;
						default:
							facingDirProxy = facingDir;
					}
				}
			}

			assert(facingDirProxy != Obj::EntityFacingDir::Invalid);

			if(facingDirProxy == Obj::EntityFacingDir::Up && myRow < gridRows - 1){
				const int upIndex = (myRow + 1) * gridCols + myCol;
				myVec.emplace_back(std::make_pair(myPair.first + 1, upIndex));
			}
			if(facingDirProxy == Obj::EntityFacingDir::Down && myRow > 0){
				const int downIndex = (myRow - 1) * gridCols + myCol;
				myVec.emplace_back(std::make_pair(myPair.first + 1, downIndex));
			}
			if(facingDirProxy == Obj::EntityFacingDir::Left && myCol > 0){
				const int leftIndex = myRow * gridCols + (myCol - 1);
				myVec.emplace_back(std::make_pair(myPair.first + 1, leftIndex));
			}
			if(facingDirProxy == Obj::EntityFacingDir::Right && myCol < gridCols - 1){
				const int rightIndex = myRow * gridCols + (myCol + 1);
				myVec.emplace_back(std::make_pair(myPair.first + 1, rightIndex));
			}

			if(gridType == HexGrid<float>::GridType::FlatTop){
				if(myCol & 1){
					if(facingDirProxy == Obj::EntityFacingDir::UL && myCol > 0 && myRow < gridRows - 1){
						const int ULIndex = (myRow + 1) * gridCols + (myCol - 1);
						myVec.emplace_back(std::make_pair(myPair.first + 1, ULIndex));
					}
					if(facingDirProxy == Obj::EntityFacingDir::UR && myCol < gridCols - 1 && myRow < gridRows - 1){
						const int URIndex = (myRow + 1) * gridCols + (myCol + 1);
						myVec.emplace_back(std::make_pair(myPair.first + 1, URIndex));
					}
				} else{
					if(facingDirProxy == Obj::EntityFacingDir::DL && myCol > 0 && myRow > 0){
						const int DLIndex = (myRow - 1) * gridCols + (myCol - 1);
						myVec.emplace_back(std::make_pair(myPair.first + 1, DLIndex));
					}
					if(facingDirProxy == Obj::EntityFacingDir::DR && myCol < gridCols - 1 && myRow > 0){
						const int DRIndex = (myRow - 1) * gridCols + (myCol + 1);
						myVec.emplace_back(std::make_pair(myPair.first + 1, DRIndex));
					}
				}
			} else{
				if(myRow & 1){
					if(facingDirProxy == Obj::EntityFacingDir::DR && myCol < gridCols - 1 && myRow > 0){
						const int DRIndex = (myRow - 1) * gridCols + (myCol + 1);
						myVec.emplace_back(std::make_pair(myPair.first + 1, DRIndex));
					}
					if(facingDirProxy == Obj::EntityFacingDir::UR && myCol < gridCols - 1 && myRow < gridRows - 1){
						const int URIndex = (myRow + 1) * gridCols + (myCol + 1);
						myVec.emplace_back(std::make_pair(myPair.first + 1, URIndex));
					}
				} else{
					if(facingDirProxy == Obj::EntityFacingDir::DL && myCol > 0 && myRow > 0){
						const int DLIndex = (myRow - 1) * gridCols + (myCol - 1);
						myVec.emplace_back(std::make_pair(myPair.first + 1, DLIndex));
					}
					if(facingDirProxy == Obj::EntityFacingDir::UL && myCol > 0 && myRow < gridRows - 1){
						const int ULIndex = (myRow + 1) * gridCols + (myCol - 1);
						myVec.emplace_back(std::make_pair(myPair.first + 1, ULIndex));
					}
				}
			}
		}

		myVec.erase(myVec.begin());
	}
}

void Scene::ClearFogOmnidirectional(const int row, const int col, const int range){
	std::vector<std::pair<int, int>> myVec;
	myVec.emplace_back(std::make_pair(0, row * gridCols + col));

	while(!myVec.empty()){
		const std::pair<int, int> myPair = myVec.front();
		if(!fogLayer[myPair.second]){
			myVec.erase(myVec.begin());
			continue;
		}
		fogLayer[myPair.second] = false;

		if(myPair.first != range){
			const int myCol = myPair.second % gridCols;
			const int myRow = myPair.second / gridCols;

			if(myRow < gridRows - 1){ //Up
				myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow + 1) * gridCols + myCol));
			}
			if(myRow > 0){ //Down
				myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow - 1) * gridCols + myCol));
			}
			if(myCol > 0){ //Left
				myVec.emplace_back(std::make_pair(myPair.first + 1, myRow * gridCols + (myCol - 1)));
			}
			if(myCol < gridCols - 1){ //Right
				myVec.emplace_back(std::make_pair(myPair.first + 1, myRow * gridCols + (myCol + 1)));
			}

			if(gridType == HexGrid<float>::GridType::FlatTop){
				if(myCol & 1){
					if(myCol > 0 && myRow < gridRows - 1){ //UL
						myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow + 1) * gridCols + (myCol - 1)));
					}
					if(myCol < gridCols - 1 && myRow < gridRows - 1){ //UR
						myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow + 1) * gridCols + (myCol + 1)));
					}
				} else{
					if(myCol > 0 && myRow > 0){ //DL
						myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow - 1) * gridCols + (myCol - 1)));
					}
					if(myCol < gridCols - 1 && myRow > 0){ //DR
						myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow - 1) * gridCols + (myCol + 1)));
					}
				}
			} else{
				if(myRow & 1){
					if(myCol < gridCols - 1 && myRow > 0){ //DR
						myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow - 1) * gridCols + (myCol + 1)));
					}
					if(myCol < gridCols - 1 && myRow < gridRows - 1){ //UR
						myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow + 1) * gridCols + (myCol + 1)));
					}
				} else{
					if(myCol > 0 && myRow > 0){ //DL
						myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow - 1) * gridCols + (myCol - 1)));
					}
					if(myCol > 0 && myRow < gridRows - 1){ //UL
						myVec.emplace_back(std::make_pair(myPair.first + 1, (myRow + 1) * gridCols + (myCol - 1)));
					}
				}
			}
		}

		myVec.erase(myVec.begin());
	}
}

int Scene::OnEvent(Event* myEvent, const bool destroyEvent){
	if(!myEvent){
		return -1;
	}

	int val = -1;

	switch(myEvent->GetID()){
		case EventID::EventAddCredits: {
			const EventAddCredits* const eventAddCredits = static_cast<const EventAddCredits*>(myEvent);

			const int credits = eventAddCredits->GetCredits();
			(eventAddCredits->GetToPlayer() ? creditsPlayer : creditsAI) += credits;

			break;
		}
		case EventID::EventDespawnEntity: {
			const EventDespawnEntity* const eventDespawnEntity = static_cast<const EventDespawnEntity*>(myEvent);
			entitiesToDeactivate.emplace_back(eventDespawnEntity->AcquireEntity());
			break;
		}
		case EventID::EventResetSelected: {
			selectedRow = selectedCol = -1;
			selectedTargetRow = selectedTargetCol = -1;
			break;
		}
	}

	if(destroyEvent && myEvent){
		delete myEvent;
		myEvent = nullptr;
	}
	return val;
}