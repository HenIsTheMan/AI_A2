#include "Scene.h"
#include "GL\glew.h"
#include "App.h"
#include "SpriteAni.h"

#include "EventBlockPlaced.h"
#include "EventGridHeightShrinking.h"
#include "EventGridWidthShrinking.h"
#include "EventCalcActiveObjs.h"
#include "EventCalcInactiveObjs.h"
#include "EventCalcAlphaCount.h"
#include "EventCalcOmegaCount.h"

#include "ListenerFlags.hpp"

#include "StateSkeleIdle.h"
#include "StateSkelePatrol.h"
#include "StateSkeleAttack.h"
#include "StateSkeleDead.h"
#include "StateSkeleChase.h"
#include "StateSkeleDeadButNotDead.h"

#include "StateReptileIdle.h"
#include "StateReptilePatrol.h"
#include "StateReptileAttack.h"
#include "StateReptileDead.h"
#include "StateReptileChase.h"
#include "StateReptileProcreate.h"

#include "StateBoyIdle.h"
#include "StateBoyPatrol.h"
#include "StateBoyAttack.h"
#include "StateBoyDead.h"
#include "StateBoyChase.h"
#include "StateBoyImmune.h"

#include "StateOrcIdle.h"
#include "StateOrcPatrol.h"
#include "StateOrcAttack.h"
#include "StateOrcDead.h"
#include "StateOrcChase.h"
#include "StateOrcExplosive.h"

extern int windowWidth;
extern int windowHeight;

Scene::Scene():
	gridCellWidth(40.0f),
	gridCellHeight(40.0f),
	gridLineThickness(2.5f),
	gridRows(20),
	gridCols(20),
	gridMinCellWidth(20.0f),
	gridMaxCellWidth(60.0f),
	gridMinCellHeight(20.0f),
	gridMaxCellHeight(60.0f),
	gridMinLineThickness(0.5f),
	gridMaxLineThickness(4.0f),
	gridMinRows(0),
	gridMinCols(0),
	gridMaxRows(30),
	gridMaxCols(30),
	grid(Grid<float>(0.0f, 0.0f, 0.0f, 0, 0)),
	simStarted(false),
	simEnded(false),
	showLines(false),
	isDay(true),
	dayNightBT(0.0f),
	gameSpd(1.0f),
	spawningTime(0.0f),
	spawningEndTime(300.0f),
	teamRegionsCase(0),
	alphaTeamLocalXStart(0),
	alphaTeamLocalXEnd(0),
	alphaTeamLocalYStart(0),
	alphaTeamLocalYEnd(0),
	omegaTeamLocalXStart(0),
	omegaTeamLocalXEnd(0),
	omegaTeamLocalYStart(0),
	omegaTeamLocalYEnd(0),
	alphaTeamEntityCount(0),
	omegaTeamEntityCount(0),
	alphaTeamSpawnLimit(50),
	omegaTeamSpawnLimit(50),
	objPool(new ObjPool<Entity>()),
	publisher(Publisher::RetrieveGlobalObjPtr()),
	skeleSM(new SM()),
	reptileSM(new SM()),
	boySM(new SM()),
	orcSM(new SM())
{
	Math::InitRNG();

	objPool->CreateObjs(2000);

	skeleSM->AddState(new State(StateID::StateSkeleIdle, StateSkeleIdle::Enter, StateSkeleIdle::Update, StateSkeleIdle::Exit));
	skeleSM->AddState(new State(StateID::StateSkelePatrol, StateSkelePatrol::Enter, StateSkelePatrol::Update, StateSkelePatrol::Exit));
	skeleSM->AddState(new State(StateID::StateSkeleAttack, StateSkeleAttack::Enter, StateSkeleAttack::Update, StateSkeleAttack::Exit));
	skeleSM->AddState(new State(StateID::StateSkeleDead, StateSkeleDead::Enter, StateSkeleDead::Update, StateSkeleDead::Exit));
	skeleSM->AddState(new State(StateID::StateSkeleChase, StateSkeleChase::Enter, StateSkeleChase::Update, StateSkeleChase::Exit));
	skeleSM->AddState(new State(StateID::StateSkeleDeadButNotDead, StateSkeleDeadButNotDead::Enter, StateSkeleDeadButNotDead::Update, StateSkeleDeadButNotDead::Exit));

	reptileSM->AddState(new State(StateID::StateReptileIdle, StateReptileIdle::Enter, StateReptileIdle::Update, StateReptileIdle::Exit));
	reptileSM->AddState(new State(StateID::StateReptilePatrol, StateReptilePatrol::Enter, StateReptilePatrol::Update, StateReptilePatrol::Exit));
	reptileSM->AddState(new State(StateID::StateReptileAttack, StateReptileAttack::Enter, StateReptileAttack::Update, StateReptileAttack::Exit));
	reptileSM->AddState(new State(StateID::StateReptileDead, StateReptileDead::Enter, StateReptileDead::Update, StateReptileDead::Exit));
	reptileSM->AddState(new State(StateID::StateReptileChase, StateReptileChase::Enter, StateReptileChase::Update, StateReptileChase::Exit));
	reptileSM->AddState(new State(StateID::StateReptileProcreate, StateReptileProcreate::Enter, StateReptileProcreate::Update, StateReptileProcreate::Exit));

	boySM->AddState(new State(StateID::StateBoyIdle, StateBoyIdle::Enter, StateBoyIdle::Update, StateBoyIdle::Exit));
	boySM->AddState(new State(StateID::StateBoyPatrol, StateBoyPatrol::Enter, StateBoyPatrol::Update, StateBoyPatrol::Exit));
	boySM->AddState(new State(StateID::StateBoyAttack, StateBoyAttack::Enter, StateBoyAttack::Update, StateBoyAttack::Exit));
	boySM->AddState(new State(StateID::StateBoyDead, StateBoyDead::Enter, StateBoyDead::Update, StateBoyDead::Exit));
	boySM->AddState(new State(StateID::StateBoyChase, StateBoyChase::Enter, StateBoyChase::Update, StateBoyChase::Exit));
	boySM->AddState(new State(StateID::StateBoyImmune, StateBoyImmune::Enter, StateBoyImmune::Update, StateBoyImmune::Exit));

	orcSM->AddState(new State(StateID::StateOrcIdle, StateOrcIdle::Enter, StateOrcIdle::Update, StateOrcIdle::Exit));
	orcSM->AddState(new State(StateID::StateOrcPatrol, StateOrcPatrol::Enter, StateOrcPatrol::Update, StateOrcPatrol::Exit));
	orcSM->AddState(new State(StateID::StateOrcAttack, StateOrcAttack::Enter, StateOrcAttack::Update, StateOrcAttack::Exit));
	orcSM->AddState(new State(StateID::StateOrcDead, StateOrcDead::Enter, StateOrcDead::Update, StateOrcDead::Exit));
	orcSM->AddState(new State(StateID::StateOrcChase, StateOrcChase::Enter, StateOrcChase::Update, StateOrcChase::Exit));
	orcSM->AddState(new State(StateID::StateOrcExplosive, StateOrcExplosive::Enter, StateOrcExplosive::Update, StateOrcExplosive::Exit));

	publisher->AddListener((long int)ListenerFlags::ObjPool, objPool);
	publisher->AddListener((long int)ListenerFlags::Scene, this);

	grid.SetCellWidth(gridCellWidth);
	grid.SetCellHeight(gridCellHeight);
	grid.SetLineThickness(gridLineThickness);
	grid.SetRows(gridRows);
	grid.SetCols(gridCols);

	isDay = bool(rand() & 1);
}

Scene::~Scene(){
	if(objPool){
		delete objPool;
		objPool = nullptr;
	}

	if(publisher){
		publisher->Destroy();
		publisher = nullptr;
	}

	if(skeleSM){
		delete skeleSM;
		skeleSM = nullptr;
	}
	if(reptileSM){
		delete reptileSM;
		reptileSM = nullptr;
	}
	if(boySM){
		delete boySM;
		boySM = nullptr;
	}
	if(orcSM){
		delete orcSM;
		orcSM = nullptr;
	}
}

void Scene::Update(double dt){
	SceneSupport::Update(dt);

	if(simEnded){
		return;
	}
	if(spawningTime >= spawningEndTime && (!alphaTeamEntityCount || !omegaTeamEntityCount)){
		simEnded = true;
		im_Cam.Reset();
		orthoProjectionScaleFactor = 1.0;
	}

	if(simStarted){
		spawningTime += (float)dt;

		if(dayNightBT <= elapsedTime){
			isDay = !isDay;
			teamRegionsCase = Math::RandIntMinMax(1, 4);
			dayNightBT = elapsedTime + 7.0f;
		}

		static bool isKeyDownZ = false;
		static bool isKeyDownX = false;
		if(!isKeyDownZ && App::Key('Z')){
			gameSpd += 0.1f;
			isKeyDownZ = true;
		} else if(isKeyDownZ && !App::Key('Z')){
			isKeyDownZ = false;
		}
		if(!isKeyDownX && App::Key('X')){
			gameSpd -= 0.1f;
			isKeyDownX = true;
		} else if(isKeyDownX && !App::Key('X')){
			isKeyDownX = false;
		}
		gameSpd = Math::Clamp(gameSpd, 0.2f, 4.0f);

		static bool isKeyDownL = false;
		if(!isKeyDownL && App::Key('L')){
			showLines = !showLines;
			isKeyDownL = true;
		} else if(isKeyDownL && !App::Key('L')){
			isKeyDownL = false;
		}

		static bool isKeyDownC = false;
		static bool isKeyDownV = false;
		if(!isKeyDownC && App::Key('C')){
			spawningEndTime += 10.0f;
			isKeyDownC = true;
		} else if(isKeyDownC && !App::Key('C')){
			isKeyDownC = false;
		}
		if(!isKeyDownV && App::Key('V')){
			spawningEndTime -= 10.0f;
			isKeyDownV = true;
		} else if(isKeyDownV && !App::Key('V')){
			isKeyDownV = false;
		}
		spawningEndTime = Math::Max(0.0f, spawningEndTime);

		static bool isKeyDownU = false;
		if(!isKeyDownU && App::Key('U')){
			++alphaTeamSpawnLimit;
			isKeyDownU = true;
		} else if(isKeyDownU && !App::Key('U')){
			isKeyDownU = false;
		}
		static bool isKeyDownI = false;
		if(!isKeyDownI && App::Key('I')){
			--alphaTeamSpawnLimit;
			isKeyDownI = true;
		} else if(isKeyDownI && !App::Key('I')){
			isKeyDownI = false;
		}
		static bool isKeyDownO = false;
		if(!isKeyDownO && App::Key('O')){
			++omegaTeamSpawnLimit;
			isKeyDownO = true;
		} else if(isKeyDownO && !App::Key('O')){
			isKeyDownO = false;
		}
		static bool isKeyDownP = false;
		if(!isKeyDownP && App::Key('P')){
			--omegaTeamSpawnLimit;
			isKeyDownP = true;
		} else if(isKeyDownP && !App::Key('P')){
			isKeyDownP = false;
		}
		alphaTeamSpawnLimit = Math::Clamp(alphaTeamSpawnLimit, 0, 500);
		omegaTeamSpawnLimit = Math::Clamp(omegaTeamSpawnLimit, 0, 500);

		UpdateGridAttribs();
		UpdateGridBlockData();
		UpdateStates();
		UpdateEntities(dt * gameSpd);
		OnEvent(FetchEvent(), true);
	} else if(App::Key(VK_SPACE)){
		simStarted = true;
		dayNightBT = elapsedTime + 7.0f;

		teamRegionsCase = Math::RandIntMinMax(1, 4);
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

	if(simEnded){
		RenderBG();

		Mesh* const textMesh = meshList[(int)GeoType::Text];
		const float textSize = (float)windowHeight * 0.05f;
		std::string endText;
		Color endColor;

		if(!alphaTeamEntityCount && !omegaTeamEntityCount){
			endText = "Draw!";
			endColor = Color(0.5f, 0.5f, 0.5f);
		} else if(alphaTeamEntityCount){
			endText = "Alpha Wins!";
			endColor = Color(1.0f, 1.0f, 1.0f);
		} else{
			endText = "Omega Wins!";
			endColor = Color(0.0f, 0.0f, 0.0f);
		}

		RenderTextOnScreen(
			textMesh,
			endText,
			endColor,
			((cosf(elapsedTime * 4.0f) + 1.0f) * 0.5f + 1.0f) * 0.5f * textSize * 2.0f,
			(float)windowWidth * 0.5f,
			textSize * 10.0f,
			TextAlignment::Center
		);
	} else{
		RenderBG();
		if(simStarted){
			RenderGrid();
			RenderRegions();
			RenderGridBlockData();
			RenderTranslucentBlock();
			RenderEntities();
		} else{
			RenderMystery();
		}
		RenderSceneText();
	}

	modelStack.PopMatrix();
}

Entity* Scene::CreateSkele(const CreateEntityParams& params) const{
	Entity* entity = objPool->RetrieveActivatedObj();

	entity->SetType(Obj::EntityType::Skele);
	entity->SetLocalPos(params.im_LocalPos);
	entity->SetLocalScale(1.0f, 1.0f, 1.0f);

	entity->SetDmg(25.0f);
	entity->SetMaxHealth(70.0f);
	entity->SetCurrHealth(entity->GetMaxHealth());
	entity->SetSpd(0.0f);

	entity->SetTarget(nullptr);
	ChooseRandDir(entity);
	entity->SetTimeLeft(0.0f);
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.0f);
	entity->SetTeam(EntityTeam::Amt);

	entity->SetStateMachine(skeleSM);
	entity->SetCurrState(skeleSM->GetState(StateID::StateSkeleIdle));
	entity->SetNextState(skeleSM->GetState(StateID::StateSkeleIdle));
	entity->SetSpriteAniMiddleName("Static");

	return entity;
}

Entity* Scene::CreateReptile(const CreateEntityParams& params) const{
	Entity* entity = objPool->RetrieveActivatedObj();

	entity->SetType(Obj::EntityType::Reptile);
	entity->SetLocalPos(params.im_LocalPos);
	entity->SetLocalScale(1.0f, 1.0f, 1.0f);

	entity->SetDmg(10.0f);
	entity->SetMaxHealth(150.0f);
	entity->SetCurrHealth(entity->GetMaxHealth());
	entity->SetSpd(0.0f);

	entity->SetTarget(nullptr);
	ChooseRandDir(entity);
	entity->SetTimeLeft(0.0f);
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.0f);
	entity->SetTeam(EntityTeam::Amt);

	entity->SetStateMachine(reptileSM);
	entity->SetCurrState(reptileSM->GetState(StateID::StateReptileIdle));
	entity->SetNextState(reptileSM->GetState(StateID::StateReptileIdle));
	entity->SetSpriteAniMiddleName("Static");

	return entity;
}

Entity* Scene::CreateBoy(const CreateEntityParams& params) const{
	Entity* entity = objPool->RetrieveActivatedObj();

	entity->SetType(Obj::EntityType::Boy);
	entity->SetLocalPos(params.im_LocalPos);
	entity->SetLocalScale(1.0f, 1.0f, 1.0f);

	entity->SetDmg(15.0f);
	entity->SetMaxHealth(120.0f);
	entity->SetCurrHealth(entity->GetMaxHealth());
	entity->SetSpd(0.0f);

	entity->SetTarget(nullptr);
	ChooseRandDir(entity);
	entity->SetTimeLeft(0.0f);
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.0f);
	entity->SetTeam(EntityTeam::Amt);

	entity->SetStateMachine(boySM);
	entity->SetCurrState(boySM->GetState(StateID::StateBoyIdle));
	entity->SetNextState(boySM->GetState(StateID::StateBoyIdle));
	entity->SetSpriteAniMiddleName("Static");

	return entity;
}

Entity* Scene::CreateOrc(const CreateEntityParams& params) const{
	Entity* entity = objPool->RetrieveActivatedObj();

	entity->SetType(Obj::EntityType::Orc);
	entity->SetLocalPos(params.im_LocalPos);
	entity->SetLocalScale(1.0f, 1.0f, 1.0f);

	entity->SetDmg(20.0f);
	entity->SetMaxHealth(100.0f);
	entity->SetCurrHealth(entity->GetMaxHealth());
	entity->SetSpd(0.0f);

	entity->SetTarget(nullptr);
	ChooseRandDir(entity);
	entity->SetTimeLeft(0.0f);
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.0f);
	entity->SetTeam(EntityTeam::Amt);

	entity->SetStateMachine(orcSM);
	entity->SetCurrState(orcSM->GetState(StateID::StateOrcIdle));
	entity->SetNextState(orcSM->GetState(StateID::StateOrcIdle));
	entity->SetSpriteAniMiddleName("Static");

	return entity;
}

void Scene::SpawnEntity(const Obj::EntityType type, const ListenerFlags& teamFlag){
	if(gridRows < 2 || gridCols < 2){
		return;
	}

	const bool team = teamFlag == ListenerFlags::AlphaTeam;
	const std::vector<std::vector<bool>>& gridBlockData = grid.GetBlockData();
	const std::vector<std::vector<bool>>& gridEntityData = grid.GetEntityData();
	const Vector3 localPos = team ? Vector3(
		(float)Math::RandIntMinMax(alphaTeamLocalXStart, alphaTeamLocalXEnd),
		(float)Math::RandIntMinMax(alphaTeamLocalYStart, alphaTeamLocalYEnd),
		0.0f
	) : Vector3((float)Math::RandIntMinMax(omegaTeamLocalXStart, omegaTeamLocalXEnd), (float)Math::RandIntMinMax(omegaTeamLocalYStart, omegaTeamLocalYEnd), 0.0f);

	if(gridBlockData[(int)localPos.y][(int)localPos.x] || gridEntityData[(int)localPos.y][(int)localPos.x]){ //If grid cell is not empty...
		return;
	}

	switch(type){
		using Obj::EntityType;

		case EntityType::Skele: {
			Entity* const skele = CreateSkele({
				localPos
			});

			skele->SetTeam(team ? EntityTeam::Alpha : EntityTeam::Omega);
			publisher->AddListener((long int)ListenerFlags::Skele | (long int) teamFlag | (long int)ListenerFlags::Entity, skele);

			break;
		}
		case EntityType::Reptile: {
			Entity* const reptile = CreateReptile({
				localPos
			});

			reptile->SetTeam(team ? EntityTeam::Alpha : EntityTeam::Omega);
			publisher->AddListener((long int)ListenerFlags::Reptile | (long int) teamFlag | (long int)ListenerFlags::Entity, reptile);

			break;
		}
		case EntityType::Boy: {
			Entity* const boy = CreateBoy({
				localPos
			});

			boy->SetTeam(team ? EntityTeam::Alpha : EntityTeam::Omega);
			publisher->AddListener((long int)ListenerFlags::Boy | (long int) teamFlag | (long int)ListenerFlags::Entity, boy);

			break;
		}
		case EntityType::Orc: {
			Entity* const orc = CreateOrc({
				localPos
			});

			orc->SetTeam(team ? EntityTeam::Alpha : EntityTeam::Omega);
			publisher->AddListener((long int)ListenerFlags::Orc | (long int) teamFlag | (long int)ListenerFlags::Entity, orc);

			break;
		}
	}
}

void Scene::UpdateGridAttribs(){
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
		if(!isKeyDown1 && App::Key('1')){
			if(gridCellWidth < gridMaxCellWidth){
				grid.SetCellWidth(++gridCellWidth);
			}
			isKeyDown1 = true;
		} else if(isKeyDown1 && !App::Key('1')){
			isKeyDown1 = false;
		}
		if(!isKeyDown2 && App::Key('2')){
			if(gridCellWidth > gridMinCellWidth){
				grid.SetCellWidth(--gridCellWidth);
			}
			isKeyDown2 = true;
		} else if(isKeyDown2 && !App::Key('2')){
			isKeyDown2 = false;
		}
		if(!isKeyDown3 && App::Key('3')){
			if(gridCellHeight < gridMaxCellHeight){
				grid.SetCellHeight(++gridCellHeight);
			}
			isKeyDown3 = true;
		} else if(isKeyDown3 && !App::Key('3')){
			isKeyDown3 = false;
		}
		if(!isKeyDown4 && App::Key('4')){
			if(gridCellHeight > gridMinCellHeight){
				grid.SetCellHeight(--gridCellHeight);
			}
			isKeyDown4 = true;
		} else if(isKeyDown4 && !App::Key('4')){
			isKeyDown4 = false;
		}
		if(!isKeyDown5 && App::Key('5')){
			if(gridLineThickness < gridMaxLineThickness){
				gridLineThickness += 0.1f;
				grid.SetLineThickness(gridLineThickness);
			}
			isKeyDown5 = true;
		} else if(isKeyDown5 && !App::Key('5')){
			isKeyDown5 = false;
		}
		if(!isKeyDown6 && App::Key('6')){
			if(gridLineThickness > gridMinLineThickness){
				gridLineThickness -= 0.1f;
				grid.SetLineThickness(gridLineThickness);
			}
			isKeyDown6 = true;
		} else if(isKeyDown6 && !App::Key('6')){
			isKeyDown6 = false;
		}
		if(!isKeyDown7 && App::Key('7')){
			if(gridRows < gridMaxRows){
				grid.SetRows(++gridRows);
			}
			isKeyDown7 = true;
		} else if(isKeyDown7 && !App::Key('7')){
			isKeyDown7 = false;
		}
		if(!isKeyDown8 && App::Key('8')){
			if(gridRows > gridMinRows){
				publisher->Notify((long int)ListenerFlags::Entity, new EventGridHeightShrinking(--gridRows), true);
				grid.SetRows(gridRows);
			}
			isKeyDown8 = true;
		} else if(isKeyDown8 && !App::Key('8')){
			isKeyDown8 = false;
		}
		if(!isKeyDown9 && App::Key('9')){
			if(gridCols < gridMaxCols){
				grid.SetCols(++gridCols);
			}
			isKeyDown9 = true;
		} else if(isKeyDown9 && !App::Key('9')){
			isKeyDown9 = false;
		}
		if(!isKeyDown0 && App::Key('0')){
			if(gridCols > gridMinCols){
				publisher->Notify((long int)ListenerFlags::Entity, new EventGridWidthShrinking(--gridCols), true);
				grid.SetCols(gridCols);
			}
			isKeyDown0 = true;
		} else if(isKeyDown0 && !App::Key('0')){
			isKeyDown0 = false;
		}
}

void Scene::UpdateGridBlockData(){
	static bool isLMB = false;
	if(!isLMB && App::IsMousePressed(0)){
		isLMB = true;
	} else if(isLMB && !App::IsMousePressed(0)){
		isLMB = false;
	}

	static bool isRMB = false;
	if(!isRMB && App::IsMousePressed(1)){
		isRMB = true;
	} else if(isRMB && !App::IsMousePressed(1)){
		isRMB = false;
	}

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

	const float gridWidth = grid.CalcWidth();
	const float gridHeight = grid.CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f;
	const float unitX = gridCellWidth + gridLineThickness;
	const float unitY = gridCellHeight + gridLineThickness;

	const float mouseRow = std::floor(((float)windowHeight - trueMouseY - yOffset - gridLineThickness * 0.5f) / unitY);
	const float mouseCol = std::floor((trueMouseX - xOffset - gridLineThickness * 0.5f) / unitX);
	const float xTranslate = mouseCol * unitX
		+ xOffset
		+ gridCellWidth * 0.5f
		+ gridLineThickness;
	const float yTranslate = mouseRow * unitY
		+ yOffset
		+ gridCellHeight * 0.5f
		+ gridLineThickness;

	if(trueMouseX > xOffset + gridLineThickness * 0.5f && trueMouseX < xOffset + gridWidth - gridLineThickness * 0.5f
		&& trueMouseY > yOffset + gridLineThickness * 0.5f && trueMouseY < yOffset + gridHeight - gridLineThickness * 0.5f){
		if(isLMB){
			grid.SetBlockData(true, (ptrdiff_t)mouseRow, (ptrdiff_t)mouseCol);
			publisher->Notify((long int)ListenerFlags::Entity, new EventBlockPlaced((int)mouseRow, (int)mouseCol), true);
		} else if(isRMB){
			grid.SetBlockData(false, (ptrdiff_t)mouseRow, (ptrdiff_t)mouseCol);
		}
	}
}

void Scene::UpdateStates(){
	UpdateSkeleStates();
	UpdateReptileStates();
	UpdateBoyStates();
	UpdateOrcStates();
}

void Scene::UpdateSkeleStates(){
	StateSkeleIdle* const stateSkeleIdle = ((StateSkeleIdle*)skeleSM->GetState(StateID::StateSkeleIdle));
	stateSkeleIdle->im_ElapsedTime = elapsedTime;
	stateSkeleIdle->im_Grid = &grid;

	StateSkelePatrol* const stateSkelePatrol = ((StateSkelePatrol*)skeleSM->GetState(StateID::StateSkelePatrol));
	stateSkelePatrol->im_ElapsedTime = elapsedTime;
	stateSkelePatrol->im_Grid = &grid;

	StateSkeleChase* const stateSkeleChase = ((StateSkeleChase*)skeleSM->GetState(StateID::StateSkeleChase));
	stateSkeleChase->im_Grid = &grid;

	StateSkeleDead* const stateSkeleDead = ((StateSkeleDead*)skeleSM->GetState(StateID::StateSkeleDead));
	stateSkeleDead->im_ObjPool = objPool;
}

void Scene::UpdateReptileStates(){
	StateReptileIdle* const stateReptileIdle = ((StateReptileIdle*)reptileSM->GetState(StateID::StateReptileIdle));
	stateReptileIdle->im_ElapsedTime = elapsedTime;
	stateReptileIdle->im_Grid = &grid;

	StateReptilePatrol* const stateReptilePatrol = ((StateReptilePatrol*)reptileSM->GetState(StateID::StateReptilePatrol));
	stateReptilePatrol->im_ElapsedTime = elapsedTime;
	stateReptilePatrol->im_Grid = &grid;

	static float updateCommonDirBT = 0.0f;
	if(updateCommonDirBT <= elapsedTime){
		ChooseRandPairOfPerpendicularDirs(stateReptilePatrol->im_CommonDirs);
		updateCommonDirBT = elapsedTime + 1.0f;
	}

	StateReptileChase* const stateReptileChase = ((StateReptileChase*)reptileSM->GetState(StateID::StateReptileChase));
	stateReptileChase->im_Grid = &grid;

	StateReptileDead* const stateReptileDead = ((StateReptileDead*)reptileSM->GetState(StateID::StateReptileDead));
	stateReptileDead->im_ObjPool = objPool;
}

void Scene::UpdateBoyStates(){
	StateBoyIdle* const stateBoyIdle = ((StateBoyIdle*)boySM->GetState(StateID::StateBoyIdle));
	stateBoyIdle->im_ElapsedTime = elapsedTime;
	stateBoyIdle->im_Grid = &grid;

	StateBoyPatrol* const stateBoyPatrol = ((StateBoyPatrol*)boySM->GetState(StateID::StateBoyPatrol));
	stateBoyPatrol->im_ElapsedTime = elapsedTime;
	stateBoyPatrol->im_Grid = &grid;

	StateBoyChase* const stateBoyChase = ((StateBoyChase*)boySM->GetState(StateID::StateBoyChase));
	stateBoyChase->im_Grid = &grid;

	StateBoyDead* const stateBoyDead = ((StateBoyDead*)boySM->GetState(StateID::StateBoyDead));
	stateBoyDead->im_ObjPool = objPool;
}

void Scene::UpdateOrcStates(){
	StateOrcIdle* const stateOrcIdle = ((StateOrcIdle*)orcSM->GetState(StateID::StateOrcIdle));
	stateOrcIdle->im_ElapsedTime = elapsedTime;
	stateOrcIdle->im_Grid = &grid;

	StateOrcPatrol* const stateOrcPatrol = ((StateOrcPatrol*)orcSM->GetState(StateID::StateOrcPatrol));
	stateOrcPatrol->im_ElapsedTime = elapsedTime;
	stateOrcPatrol->im_Grid = &grid;

	StateOrcChase* const stateOrcChase = ((StateOrcChase*)orcSM->GetState(StateID::StateOrcChase));
	stateOrcChase->im_Grid = &grid;

	StateOrcDead* const stateOrcDead = ((StateOrcDead*)orcSM->GetState(StateID::StateOrcDead));
	stateOrcDead->im_ObjPool = objPool;
}

void Scene::UpdateGridEntityData(){
	std::vector<std::vector<bool>>& gridEntityData = grid.RetrieveEntityData();
	for(int i = 0; i < gridRows; ++i){
		for(int j = 0; j < gridCols; ++j){
			gridEntityData[i][j] = false; //All data becomes false
		}
	}

	std::vector<std::pair<bool, Entity*>>& entityPool = objPool->RetrievePool();
	const size_t entityPoolSize = entityPool.size();

	for(size_t i = 0; i < entityPoolSize; ++i){
		if(entityPool[i].first){
			Entity* const entity = entityPool[i].second;
			const StateID entityCurrStateID = entity->GetCurrState()->GetID();
			if((entityCurrStateID == StateID::StateSkeleDead)
				|| (entityCurrStateID == StateID::StateReptileDead)
				|| (entityCurrStateID == StateID::StateBoyDead)
				|| (entityCurrStateID == StateID::StateOrcDead)
			){
				continue;
			}

			const Vector3& entityLocalPos = entity->GetLocalPos();
			gridEntityData[(int)std::floorf(entityLocalPos.y)][(int)std::floorf(entityLocalPos.x)] = true;
			gridEntityData[(int)std::floorf(entityLocalPos.y)][(int)std::ceilf(entityLocalPos.x)] = true;
			gridEntityData[(int)std::ceilf(entityLocalPos.y)][(int)std::floorf(entityLocalPos.x)] = true;
			gridEntityData[(int)std::ceilf(entityLocalPos.y)][(int)std::ceilf(entityLocalPos.x)] = true;
		}
	}
}

void Scene::UpdateEntities(const double dt){
	switch(teamRegionsCase){
		case 1:
			alphaTeamLocalXStart = 0;
			alphaTeamLocalXEnd = (int)std::floorf((float)gridCols * 0.5f) - 1;
			alphaTeamLocalYStart = 0;
			alphaTeamLocalYEnd = gridRows - 1;
			omegaTeamLocalXStart = (int)std::ceilf((float)gridCols * 0.5f);
			omegaTeamLocalXEnd = gridCols - 1;
			omegaTeamLocalYStart = 0;
			omegaTeamLocalYEnd = gridRows - 1;
			break;
		case 2:
			alphaTeamLocalXStart = (int)std::ceilf((float)gridCols * 0.5f);
			alphaTeamLocalXEnd = gridCols - 1;
			alphaTeamLocalYStart = 0;
			alphaTeamLocalYEnd = gridRows - 1;
			omegaTeamLocalXStart = 0;
			omegaTeamLocalXEnd = (int)std::floorf((float)gridCols * 0.5f) - 1;
			omegaTeamLocalYStart = 0;
			omegaTeamLocalYEnd = gridRows - 1;
			break;
		case 3:
			alphaTeamLocalXStart = 0;
			alphaTeamLocalXEnd = gridCols - 1;
			alphaTeamLocalYStart = 0;
			alphaTeamLocalYEnd = (int)std::floorf((float)gridRows * 0.5f) - 1;
			omegaTeamLocalXStart = 0;
			omegaTeamLocalXEnd = gridCols - 1;
			omegaTeamLocalYStart = (int)std::ceilf((float)gridRows * 0.5f);
			omegaTeamLocalYEnd = gridRows - 1;
			break;
		case 4:
			alphaTeamLocalXStart = 0;
			alphaTeamLocalXEnd = gridCols - 1;
			alphaTeamLocalYStart = (int)std::ceilf((float)gridRows * 0.5f);
			alphaTeamLocalYEnd = gridRows - 1;
			omegaTeamLocalXStart = 0;
			omegaTeamLocalXEnd = gridCols - 1;
			omegaTeamLocalYStart = 0;
			omegaTeamLocalYEnd = (int)std::floorf((float)gridRows * 0.5f) - 1;
			break;
		default:
			assert(false);
	}

	static float alphaTeamSpawnBT = 0.0f;
	static float omegaTeamSpawnBT = 0.0f;
	alphaTeamEntityCount = publisher->Notify((long int)ListenerFlags::ObjPool, new EventCalcAlphaCount(), false);
	omegaTeamEntityCount = publisher->Notify((long int)ListenerFlags::ObjPool, new EventCalcOmegaCount(), false);

	if(spawningTime < spawningEndTime){
		if(alphaTeamEntityCount < alphaTeamSpawnLimit && alphaTeamSpawnBT <= elapsedTime){
			SpawnEntity((Obj::EntityType)Math::RandIntMinMax((int)Obj::EntityType::Skele, (int)Obj::EntityType::Orc), ListenerFlags::AlphaTeam);
			alphaTeamSpawnBT = elapsedTime + (!isDay ? Math::RandFloatMinMax(1.8f, 2.5f) : Math::RandFloatMinMax(0.7f, 1.5f));
		}
		if(omegaTeamEntityCount < omegaTeamSpawnLimit && omegaTeamSpawnBT <= elapsedTime){
			SpawnEntity((Obj::EntityType)Math::RandIntMinMax((int)Obj::EntityType::Skele, (int)Obj::EntityType::Orc), ListenerFlags::OmegaTeam);
			omegaTeamSpawnBT = elapsedTime + (isDay ? Math::RandFloatMinMax(1.8f, 2.5f) : Math::RandFloatMinMax(0.7f, 1.5f));
		}
	}

	std::vector<std::pair<bool, Entity*>>& entityPool = objPool->RetrievePool();
	const size_t& entityPoolSize = entityPool.size();

	for(size_t i = 0; i < entityPoolSize; ++i){
		if(entityPool[i].first){
			Entity* const entity = entityPool[i].second;

			///Process all events in event queue here as most of them will break the game if not processed here and now
			Event* myEvent = nullptr;
			do{
				myEvent = entity->FetchEvent();
				switch(entity->OnEvent(myEvent, true)){
					case -5:
						objPool->DeactivateObj(entity);
						break;
				}
			} while(myEvent);
		}
	}

	UpdateGridEntityData();

	for(size_t i = 0; i < entityPoolSize; ++i){
		if(entityPool[i].first){
			Entity* const entity = entityPool[i].second;

			switch(entity->GetType()){
				using Obj::EntityType;

				case EntityType::Skele:
					skeleSM->CheckForStateTransition(entity);
					skeleSM->UpdateCurrState(entity, dt);
					break;
				case EntityType::Reptile:
					reptileSM->CheckForStateTransition(entity);
					reptileSM->UpdateCurrState(entity, dt);
					break;
				case EntityType::Boy:
					boySM->CheckForStateTransition(entity);
					boySM->UpdateCurrState(entity, dt);
					break;
				case EntityType::Orc:
					orcSM->CheckForStateTransition(entity);
					orcSM->UpdateCurrState(entity, dt);
					break;
			}
		}
	}
}

void Scene::RenderGrid(){
	const float gridWidth = grid.CalcWidth();
	const float gridHeight = grid.CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f + gridLineThickness * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f + gridLineThickness * 0.5f;

	const int amtOfVertLines = grid.CalcAmtOfVertLines();
	for(int i = 0; i < amtOfVertLines; ++i){
		modelStack.PushMatrix();
		modelStack.Translate(
			xOffset + (gridCellWidth + gridLineThickness) * (float)i,
			(float)windowHeight * 0.5f,
			0.05f
		);
		modelStack.Scale(
			gridLineThickness,
			gridHeight,
			1.0f
		);
		RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.5f, 0.5f, 0.5f), 1.0f);
		modelStack.PopMatrix();
	}

	const int amtOfHorizLines = grid.CalcAmtOfHorizLines();
	for(int i = 0; i < amtOfHorizLines; ++i){
		modelStack.PushMatrix();
		modelStack.Translate(
			(float)windowWidth * 0.5f,
			yOffset + (gridCellHeight + gridLineThickness) * (float)i,
			0.05f
		);
		modelStack.Scale(
			gridWidth,
			gridLineThickness,
			1.0f
		);
		RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.5f, 0.5f, 0.5f), 1.0f);
		modelStack.PopMatrix();
	}
}

void Scene::RenderMystery(){
	modelStack.PushMatrix();
	modelStack.Translate(
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f,
		0.05f
	);
	modelStack.Scale(
		grid.CalcWidth(),
		grid.CalcHeight(),
		1.0f
	);
	RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.5f, 0.5f, 0.5f), 1.0f);
	modelStack.PopMatrix();
}

void Scene::RenderGridBlockData(){
	const float gridWidth = grid.CalcWidth();
	const float gridHeight = grid.CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f + gridLineThickness + gridCellWidth * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f + gridLineThickness + gridCellHeight * 0.5f;

	const std::vector<std::vector<bool>>& gridBlockData = grid.GetBlockData();

	for(int i = 0; i < gridRows; ++i){
		for(int j = 0; j < gridCols; ++j){
			if(gridBlockData[i][j]){
				modelStack.PushMatrix();
				modelStack.Translate(
					xOffset + (gridLineThickness + gridCellWidth) * (float)j,
					yOffset + (gridLineThickness + gridCellHeight) * (float)i,
					0.1f
				);
				modelStack.Scale(
					gridCellWidth,
					gridCellHeight,
					1.0f
				);
				RenderMesh(meshList[(int)GeoType::Block], false);
				modelStack.PopMatrix();
			}
		}
	}
}

void Scene::RenderEntities(){
	static std::string stateTexts[(int)StateID::Amt]{
		"StateSkeleIdle",
		"StateSkelePatrol",
		"StateSkeleAttack",
		"StateSkeleDead",
		"StateSkeleChase",
		"StateSkeleDeadButNotDead",

		"StateReptileIdle",
		"StateReptilePatrol",
		"StateReptileAttack",
		"StateReptileDead",
		"StateReptileChase",
		"StateReptileProcreate",

		"StateBoyIdle",
		"StateBoyPatrol",
		"StateBoyAttack",
		"StateBoyDead",
		"StateBoyChase",
		"StateBoyImmune",

		"StateOrcIdle",
		"StateOrcPatrol",
		"StateOrcAttack",
		"StateOrcDead",
		"StateOrcChase",
		"StateOrcExplosive",
	};

	const std::vector<std::pair<bool, Entity*>>& entityPool = objPool->GetPool();
	const size_t& entityPoolSize = entityPool.size();

	const float gridWidth = grid.CalcWidth();
	const float gridHeight = grid.CalcHeight();

	const float xPosOffset = ((float)windowWidth - gridWidth) * 0.5f + gridCellWidth * 0.5f + gridLineThickness;
	const float yPosOffset = ((float)windowHeight - gridHeight) * 0.5f + gridCellHeight * 0.5f + gridLineThickness;

	static float individualDepthOffset = 0.0f;
	float opacity;

	for(size_t i = 0; i < entityPoolSize; ++i){
		if(entityPool[i].first){
			opacity = 1.0f;
			const Entity* const entity = entityPool[i].second;
			const Vector3& entityLocalPos = entity->GetLocalPos();
			const Vector3& entityLocalScale = entity->GetLocalScale();

			Vector3 entityWorldPos = Vector3(
				xPosOffset + entityLocalPos.x * (gridCellWidth + gridLineThickness),
				yPosOffset + entityLocalPos.y * (gridCellHeight + gridLineThickness),
				0.2f + individualDepthOffset
			);
			const Vector3 entityWorldScale = Vector3(
				entityLocalScale.x * gridCellWidth,
				entityLocalScale.y * gridCellHeight,
				1.0f
			);

			///Render entity-to-target line
			if(showLines){
				glDepthFunc(GL_LEQUAL);
				const Entity* const entityTarget = entity->GetTarget();

				if(entityTarget){
					const Vector3 vec = entity->GetTarget()->GetLocalPos() - entity->GetLocalPos();
					modelStack.PushMatrix();

					modelStack.Translate(
						entityWorldPos.x + vec.x * 0.5f * (gridCellWidth + gridLineThickness),
						entityWorldPos.y + vec.y * 0.5f * (gridCellHeight + gridLineThickness),
						1.0f
					);
					modelStack.Rotate(
						Math::RadianToDegree(atan2(vec.y, vec.x)),
						0.0f,
						0.0f,
						1.0f
					);
					modelStack.Scale(
						vec.Length() * (gridCellWidth + gridLineThickness),
						1.0f,
						1.0f
					);

					RenderMesh(meshList[(int)GeoType::Quad], true,
						entity->GetTeam() == EntityTeam::Alpha ? Color(1.0f, 1.0f, 1.0f) : Color(0.0f, 0.0f, 0.0f), 1.0f);
					modelStack.PopMatrix();
				}
				glDepthFunc(GL_LESS);
			}

			modelStack.PushMatrix();

			modelStack.Translate(
				entityWorldPos.x,
				entityWorldPos.y,
				entityWorldPos.z
			);

			modelStack.Scale(
				entityWorldScale.x,
				entityWorldScale.y,
				entityWorldScale.z
			);

			switch(entity->GetCurrState()->GetID()){
				case StateID::StateSkeleDead:
					opacity = entity->GetTimeLeft() / 4.0f;
					ManualRenderMesh("SkeleFacePlant", 1.0f, 1.0f, meshList[(int)GeoType::Skele], true, Color(), opacity);
					break;
				case StateID::StateReptileDead:
					opacity = entity->GetTimeLeft() / 6.0f;
					ManualRenderMesh("ReptileFacePlant", 1.0f, 1.0f, meshList[(int)GeoType::Reptile], true, Color(), opacity);
					break;
				case StateID::StateBoyDead:
					opacity = entity->GetTimeLeft() / 5.0f;
					ManualRenderMesh("BoyFacePlant", 1.0f, 1.0f, meshList[(int)GeoType::Boy], true, Color(), opacity);
					break;
				case StateID::StateOrcDead:
					opacity = entity->GetTimeLeft() / 7.0f;
					ManualRenderMesh("OrcFacePlant", 1.0f, 1.0f, meshList[(int)GeoType::Orc], true, Color(), opacity);
					break;
				case StateID::StateSkeleDeadButNotDead:
					ManualRenderMesh("SkeleFacePlant", 1.0f, 1.0f, meshList[(int)GeoType::Skele], true, Color(1.0f, 0.0f, 0.0f), 1.0f);
					break;
				case StateID::StateReptileProcreate:
					ManualRenderMesh("ReptileStaticDown", 1.0f, 1.0f, meshList[(int)GeoType::Reptile], true, Color(0.0f, 1.0f, 0.0f), 1.0f);
					break;
				case StateID::StateBoyImmune:
					ManualRenderMesh("BoyStaticDown", 1.0f, 1.0f, meshList[(int)GeoType::Boy], true, Color(0.0f, 0.0f, 1.0f), 1.0f);
					break;
				case StateID::StateOrcExplosive:
					ManualRenderMesh("OrcFacePlant", 1.0f, 1.0f, meshList[(int)GeoType::Orc], true, Color(1.0f, 0.0f, 0.0f), 1.0f);
					break;
				default:
					RenderEntitiesPart2(entity);
			}

			///Render health, health bar and text
			const float ratio = (float)entity->GetCurrHealth() / (float)entity->GetMaxHealth();

			modelStack.PushMatrix();

			modelStack.Translate(
				0.0f,
				0.3f,
				0.2f + individualDepthOffset
			);
			modelStack.Scale(
				0.9f,
				0.05f,
				1.0f
			);

			modelStack.PushMatrix();

			modelStack.Translate(
				0.0f,
				0.4f,
				0.0f
			);
			modelStack.Scale(
				0.2f,
				4.0f,
				1.0f
			);

			RenderText(
				entity->GetTeam() == EntityTeam::Alpha ? meshList[(int)GeoType::TextMod2] : meshList[(int)GeoType::TextMod1],
				stateTexts[(int)entity->GetCurrState()->GetID()],
				entity->GetTeam() == EntityTeam::Alpha ? Color() : Color(0.0f, 0.0f, 0.0f),
				TextAlignment::Center
			);
			modelStack.PopMatrix();

			if(entity->GetCurrHealth() > 0.0f){
				modelStack.PushMatrix();

				modelStack.Translate(
					-(1.f - ratio) * 0.5f,
					0.0f,
					0.0f
				);
				modelStack.Scale(
					ratio - 0.05f,
					0.5f,
					1.0f
				);

				RenderMesh(meshList[(int)GeoType::Quad], true, Color(ratio < 0.5f ? 1.0f : (1.0f - ratio) * 2.0f, ratio > 0.5f ? 1.0f : ratio * 2.0f, 0.0f), opacity);
				modelStack.PopMatrix();

				RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.1f, 0.1f, 0.1f), opacity);
			}
			modelStack.PopMatrix();

			modelStack.PopMatrix();

			individualDepthOffset += 0.01f;
		}
	}

	individualDepthOffset = 0.0f;
}

void Scene::RenderEntitiesPart2(const Entity* const entity){
	Mesh* spriteAniMesh = nullptr;
	std::string spriteAniName;

	switch(entity->GetType()){
		using Obj::EntityType;

		case EntityType::Skele:
			spriteAniMesh = meshList[(int)GeoType::Skele];
			spriteAniName += "Skele";
			break;
		case EntityType::Reptile:
			spriteAniMesh = meshList[(int)GeoType::Reptile];
			spriteAniName += "Reptile";
			break;
		case EntityType::Boy:
			spriteAniMesh = meshList[(int)GeoType::Boy];
			spriteAniName += "Boy";
			break;
		case EntityType::Orc:
			spriteAniMesh = meshList[(int)GeoType::Orc];
			spriteAniName += "Orc";
			break;
	}

	const std::string& entitySpriteAniMiddleName = entity->GetSpriteAniMiddleName();
	assert(entitySpriteAniMiddleName != "");

	spriteAniName += entitySpriteAniMiddleName;

	assert(spriteAniMesh != nullptr);
	assert(spriteAniName != "");

	const Vector3 vec = entity->GetGridTargetLocalPos() - entity->GetLocalPos();
	if(vec.x > 0.0f){
		spriteAniName += (std::string)"Right";
	} else if(vec.x < 0.0f){
		spriteAniName += (std::string)"Left";
	} else if(vec.y > 0.0f){
		spriteAniName += (std::string)"Up";
	} else if(vec.y < 0.0f){
		spriteAniName += (std::string)"Down";
	} else{
		spriteAniName += (std::string)"Down";
	}

	ManualRenderMesh(spriteAniName, entity->GetSpriteAniElapsedTime(), entity->GetSpriteAniDelay(), spriteAniMesh, false);
}

void Scene::RenderRegions(){
	const float gridWidth = grid.CalcWidth();
	const float gridHeight = grid.CalcHeight();

	float alphaPosX;
	float alphaPosY;
	float alphaScaleX;
	float alphaScaleY;
	float omegaPosX;
	float omegaPosY;
	float omegaScaleX;
	float omegaScaleY;
	float dividerScaleX = 0.0f;
	float dividerScaleY = 0.0f;

	switch(teamRegionsCase){
		case 1:
			alphaPosX = (float)windowWidth * 0.5f - gridWidth * 0.25f;
			alphaPosY = (float)windowHeight * 0.5f;
			alphaScaleX = gridWidth * 0.5f;
			alphaScaleY = gridHeight;

			omegaPosX = (float)windowWidth * 0.5f + gridWidth * 0.25f;
			omegaPosY = (float)windowHeight * 0.5f;
			omegaScaleX = gridWidth * 0.5f;
			omegaScaleY = gridHeight;

			dividerScaleX = gridCellWidth;
			dividerScaleY = gridHeight;
			break;
		case 2:
			alphaPosX = (float)windowWidth * 0.5f + gridWidth * 0.25f;
			alphaPosY = (float)windowHeight * 0.5f;
			alphaScaleX = gridWidth * 0.5f;
			alphaScaleY = gridHeight;

			omegaPosX = (float)windowWidth * 0.5f - gridWidth * 0.25f;
			omegaPosY = (float)windowHeight * 0.5f;
			omegaScaleX = gridWidth * 0.5f;
			omegaScaleY = gridHeight;

			dividerScaleX = gridCellWidth;
			dividerScaleY = gridHeight;
			break;
		case 3:
			alphaPosX = (float)windowWidth * 0.5f;
			alphaPosY = (float)windowHeight * 0.5f - gridHeight * 0.25f;
			alphaScaleX = gridWidth;
			alphaScaleY = gridHeight * 0.5f;

			omegaPosX = (float)windowWidth * 0.5f;
			omegaPosY = (float)windowHeight * 0.5f + gridHeight * 0.25f;
			omegaScaleX = gridWidth;
			omegaScaleY = gridHeight * 0.5f;

			dividerScaleX = gridWidth;
			dividerScaleY = gridCellHeight;
			break;
		case 4:
			alphaPosX = (float)windowWidth * 0.5f;
			alphaPosY = (float)windowHeight * 0.5f + gridHeight * 0.25f;
			alphaScaleX = gridWidth;
			alphaScaleY = gridHeight * 0.5f;

			omegaPosX = (float)windowWidth * 0.5f;
			omegaPosY = (float)windowHeight * 0.5f - gridHeight * 0.25f;
			omegaScaleX = gridWidth;
			omegaScaleY = gridHeight * 0.5f;

			dividerScaleX = gridWidth;
			dividerScaleY = gridCellHeight;
			break;
		default:
			assert(false);
	}

	///Render dead zone
	if(gridRows != 0 && gridCols != 0 && (teamRegionsCase > 2 && gridRows & 1) || (teamRegionsCase < 3 && gridCols & 1)){
		modelStack.PushMatrix();
		modelStack.Translate(
			(float)windowWidth * 0.5f,
			(float)windowHeight * 0.5f,
			0.05f
		);
		modelStack.Scale(
			dividerScaleX,
			dividerScaleY,
			1.0f
		);
		RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.5f, 0.5f, 0.5f), 0.8f);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(
		alphaPosX,
		alphaPosY,
		0.05f
	);
	modelStack.Scale(
		alphaScaleX,
		alphaScaleY,
		1.0f
	);
	RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.7f, 0.7f, 0.7f), 0.8f);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(
		omegaPosX,
		omegaPosY,
		0.05f
	);
	modelStack.Scale(
		omegaScaleX,
		omegaScaleY,
		1.0f
	);
	RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.3f, 0.3f, 0.3f), 0.8f);
	modelStack.PopMatrix();
}

void Scene::RenderTranslucentBlock(){
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

	const float gridWidth = grid.CalcWidth();
	const float gridHeight = grid.CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f;

	if(trueMouseX > xOffset + gridLineThickness * 0.5f && trueMouseX < xOffset + gridWidth - gridLineThickness * 0.5f
		&& trueMouseY > yOffset + gridLineThickness * 0.5f && trueMouseY < yOffset + gridHeight - gridLineThickness * 0.5f){
		const float unitX = gridCellWidth + gridLineThickness;
		const float unitY = gridCellHeight + gridLineThickness;

		const float mouseRow = std::floor(((float)windowHeight - trueMouseY - yOffset - gridLineThickness * 0.5f) / unitY);
		const float mouseCol = std::floor((trueMouseX - xOffset - gridLineThickness * 0.5f) / unitX);
		const float xTranslate = mouseCol * unitX
			+ xOffset
			+ gridCellWidth * 0.5f
			+ gridLineThickness;
		const float yTranslate = mouseRow * unitY
			+ yOffset
			+ gridCellHeight * 0.5f
			+ gridLineThickness;

		modelStack.PushMatrix();
			modelStack.Translate(
				xTranslate,
				yTranslate,
				0.1f
			);
			modelStack.Scale(
				gridCellWidth,
				gridCellHeight,
				1.0f
			);
			RenderMesh(meshList[(int)GeoType::Block], true, Color(), 0.5f);
		modelStack.PopMatrix();
	}
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
		if(isDay){
			RenderMesh(meshList[(int)GeoType::DayBG], false);
		} else{
			RenderMesh(meshList[(int)GeoType::NightBG], false);
		}
	modelStack.PopMatrix();
}

void Scene::RenderSceneText(){
	Mesh* const textMesh = meshList[(int)GeoType::Text];
	const float textSize = (float)windowHeight * 0.05f;
	const float colorComponent = (sinf(elapsedTime * 4.0f) + 1.0f) * 0.5f;

	if(!simStarted){
		RenderTextOnScreen(
			textMesh,
			"Press Space to start!",
			Color(colorComponent, colorComponent, colorComponent),
			((sinf(elapsedTime * 4.0f) + 1.0f) * 0.5f + 1.0f) * 0.5f * textSize * 2.0f,
			(float)windowWidth * 0.5f,
			textSize * 10.0f,
			TextAlignment::Center
		);
	}

	RenderDebugInfoText(textMesh, Color(0.0f, 1.0f, 0.0f), textSize);
	RenderControlsText(textMesh, Color(1.0f, 0.0f, 1.0f), textSize);
	RenderGridAttribsText(textMesh, Color(1.0f, 1.0f, 0.0f), textSize);
	RenderGameInfoText(textMesh, Color(1.0f, 0.5f, 0.0f), textSize);
}

void Scene::RenderDebugInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
	RenderTextOnScreen(
		textMesh,
		"Elapsed time: " + std::to_string(elapsedTime).substr(0, std::to_string((int)elapsedTime).length() + 3),
		textColor,
		textSize,
		0.0f,
		textSize * 0.0f
	);
	RenderTextOnScreen(
		textMesh,
		"FPS: " + std::to_string(FPS).substr(0, std::to_string((int)FPS).length() + 3),
		textColor,
		textSize,
		0.0f,
		textSize * 1.0f
	);
}

void Scene::RenderControlsText(Mesh* const textMesh, const Color& textColor, const float textSize){
	RenderTextOnScreen(
		textMesh,
		"F1: Toggle fullscreen",
		textColor,
		textSize,
		0.0f,
		textSize * 19.0f
	);
	RenderTextOnScreen(
		textMesh,
		"F2: Change polygon mode",
		textColor,
		textSize,
		0.0f,
		textSize * 18.0f
	);
	RenderTextOnScreen(
		textMesh,
		"Tab: ...",
		textColor,
		textSize,
		0.0f,
		textSize * 17.0f
	);
	RenderTextOnScreen(
		textMesh,
		"WASD: Move cam",
		textColor,
		textSize,
		0.0f,
		textSize * 16.0f
	);
	RenderTextOnScreen(
		textMesh,
		"R: Reset cam",
		textColor,
		textSize,
		0.0f,
		textSize * 15.0f
	);
	RenderTextOnScreen(
		textMesh,
		"Z: Increase game spd",
		textColor,
		textSize,
		0.0f,
		textSize * 14.0f
	);
	RenderTextOnScreen(
		textMesh,
		"X: Decrease game spd",
		textColor,
		textSize,
		0.0f,
		textSize * 13.0f
	);
	RenderTextOnScreen(
		textMesh,
		"L: Toggle entity-to-target lines",
		textColor,
		textSize,
		0.0f,
		textSize * 12.0f
	);
	RenderTextOnScreen(
		textMesh,
		"0 - 9: Modify grid attribs",
		textColor,
		textSize,
		0.0f,
		textSize * 11.0f
	);
	RenderTextOnScreen(
		textMesh,
		"C: Raise spawning end time",
		textColor,
		textSize,
		0.0f,
		textSize * 10.0f
	);
	RenderTextOnScreen(
		textMesh,
		"V: Lower spawning end time",
		textColor,
		textSize,
		0.0f,
		textSize * 9.0f
	);
	RenderTextOnScreen(
		textMesh,
		"U: Raise alpha spawn limit",
		textColor,
		textSize,
		0.0f,
		textSize * 8.0f
	);
	RenderTextOnScreen(
		textMesh,
		"I: Lower alpha spawn limit",
		textColor,
		textSize,
		0.0f,
		textSize * 7.0f
	);
	RenderTextOnScreen(
		textMesh,
		"O: Raise omega spawn limit",
		textColor,
		textSize,
		0.0f,
		textSize * 6.0f
	);
	RenderTextOnScreen(
		textMesh,
		"P: Lower omega spawn limit",
		textColor,
		textSize,
		0.0f,
		textSize * 5.0f
	);
}

void Scene::RenderGridAttribsText(Mesh* const textMesh, const Color& textColor, const float textSize){
	RenderTextOnScreen(
		textMesh,
		"Grid cell width: " + std::to_string(gridCellWidth).substr(0, std::to_string((int)gridCellWidth).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 0.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Grid cell height: " + std::to_string(gridCellHeight).substr(0, std::to_string((int)gridCellHeight).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 1.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Grid line thickness: " + std::to_string(gridLineThickness).substr(0, std::to_string((int)gridLineThickness).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 2.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Grid rows: " + std::to_string(gridRows),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 3.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Grid cols: " + std::to_string(gridCols),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 4.0f,
		TextAlignment::Right
	);
}

void Scene::RenderGameInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
	RenderTextOnScreen(
		textMesh,
		"Spawning time: " + std::to_string(spawningTime).substr(0, std::to_string((int)spawningTime).length() + 3),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 19.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Spawning end time: " + std::to_string(spawningEndTime).substr(0, std::to_string((int)spawningEndTime).length() + 3),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 18.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Game Spd: " + std::to_string(gameSpd).substr(0, std::to_string((int)gameSpd).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 17.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Active objs: " + std::to_string(publisher->Notify((long int)ListenerFlags::ObjPool, new EventCalcActiveObjs(), false)),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 16.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Inactive objs: " + std::to_string(publisher->Notify((long int)ListenerFlags::ObjPool, new EventCalcInactiveObjs(), false)),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 15.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Alpha count: " + std::to_string(alphaTeamEntityCount),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 14.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Omega count: " + std::to_string(omegaTeamEntityCount),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 13.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Alpha spawn limit: " + std::to_string(alphaTeamSpawnLimit),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 12.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Omega spawn limit: " + std::to_string(omegaTeamSpawnLimit),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 11.0f,
		TextAlignment::Right
	);
}

int Scene::OnEvent(Event* myEvent, const bool destroyEvent){
	if(!myEvent){
		return -1;
	}

	int val = -1;

	switch(myEvent->GetID()){
		case EventID::EventProcreated: {
			const EventProcreated* const eventProcreated = static_cast<const EventProcreated*>(myEvent);

			std::vector<Vector3> possibleOffsets{
				Vector3(1.0f, 0.0f, 0.0f),
				Vector3(-1.0f, 0.0f, 0.0f),
				Vector3(0.0f, 1.0f, 0.0f),
				Vector3(0.0f, -1.0f, 0.0f),
				Vector3(1.0f, 1.0f, 0.0f),
				Vector3(-1.0f, 1.0f, 0.0f),
				Vector3(1.0f, -1.0f, 0.0f),
				Vector3(-1.0f, -1.0f, 0.0f)
			};
			std::random_shuffle(possibleOffsets.begin(), possibleOffsets.end(), [](const int i){
				return rand() % i;
			});
			const size_t possibleOffsetsSize = possibleOffsets.size();
			const std::vector<std::vector<bool>>& gridBlockData = grid.GetBlockData();
			const std::vector<std::vector<bool>>& gridEntityData = grid.GetEntityData();

			for(size_t i = 0; i < possibleOffsetsSize; ++i){
				const Vector3 spawnLocalPos = eventProcreated->GetLocalPos() + possibleOffsets[i];
				if((int)spawnLocalPos.x < 0
					|| (int)spawnLocalPos.y < 0
					|| (int)spawnLocalPos.x >= gridCols
					|| (int)spawnLocalPos.y >= gridRows
					|| gridBlockData[(int)spawnLocalPos.y][(int)spawnLocalPos.x]
					|| gridEntityData[(int)spawnLocalPos.y][(int)spawnLocalPos.x]
				){ //If grid cell is not valid...
					continue;
				}

				Entity* const reptile = CreateReptile({
					spawnLocalPos
				});

				const ListenerFlags& teamFlag = eventProcreated->GetTeamFlag();
				const bool team = teamFlag == ListenerFlags::AlphaTeam;
				reptile->SetTeam(team ? EntityTeam::Alpha : EntityTeam::Omega);
				publisher->AddListener((long int)ListenerFlags::Reptile | (long int)teamFlag | (long int)ListenerFlags::Entity, reptile);

				val = 1;
				break;
			}
			break;
		}
	}

	if(destroyEvent && myEvent){
		delete myEvent;
		myEvent = nullptr;
	}
	return val;
}