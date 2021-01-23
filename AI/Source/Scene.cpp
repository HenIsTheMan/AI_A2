#include "Scene.h"

#include "App.h"
#include "GLFW/glfw3.h"

#include "Easing.hpp"

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
	gridType(HexGrid<float>::GridType::FlatTop),
	gridCellScaleX(48.0f),
	gridCellScaleY(48.0f),
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
	sim(new Sim()),
	grid(new HexGrid<float>(HexGrid<float>::GridType::Amt, 0.0f, 0.0f, 0.0f, 0, 0)),
	entityPool(ObjPool<Entity>::RetrieveGlobalObjPtr()),
	publisher(Publisher::RetrieveGlobalObjPtr()),
	activeEntities(entityPool->GetActiveObjs()),
	myThread(nullptr)
{
}

Scene::~Scene(){
	if(sim != nullptr){
		delete sim;
		sim = nullptr;
	}

	if(grid != nullptr){
		delete grid;
		grid = nullptr;
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
	entityPool->Init(entityPoolSize, entityPoolSize);

	sim->status = SimRuntimeStatus::Waiting;
	sim->mode = SimMode::ProtectTheKing;
	sim->spd = 1.0f;
	sim->turnDuration = 5.0f;
	sim->turnElapsedTime = 0.0f;
	sim->turn = (bool)Math::RandIntMinMax(0, 1) ? SimTurn::Player : SimTurn::AI;
	sim->timeOfDayDuration = 10.0f;
	sim->timeOfDayElapsedTime = 0.0f;
	sim->timeOfDay = (SimTimeOfDay)Math::RandIntMinMax((int)SimTimeOfDay::Day, (int)SimTimeOfDay::Amt - 1);

	grid->SetGridType(gridType);
	grid->SetCellScaleX(gridCellScaleX);
	grid->SetCellScaleY(gridCellScaleY);
	grid->SetLineThickness(gridLineThickness);
	grid->SetRows(gridRows);
	grid->SetCols(gridCols);
}

void Scene::Update(const double updateDt, const double renderDt){
	SceneSupport::Update(updateDt, renderDt);

	UpdateMisc(updateDt);

	switch(sim->status){
		case SimRuntimeStatus::Waiting: {
			UpdateGridAttribs();

			static bool isKeyDownLeft = false;
			static bool isKeyDownRight = false;

			if(!isKeyDownLeft && App::Key(VK_LEFT)){
				const int simModeInt = (int)sim->mode;
				sim->mode = simModeInt == (int)SimMode::None + 1 ? SimMode((int)SimMode::Amt - 1) : SimMode(simModeInt - 1);

				isKeyDownLeft = true;
			} else if(isKeyDownLeft && !App::Key(VK_LEFT)){
				isKeyDownLeft = false;
			}
			if(!isKeyDownRight && App::Key(VK_RIGHT)){
				const int simModeInt = (int)sim->mode;
				sim->mode = simModeInt == (int)SimMode::Amt - 1 ? SimMode((int)SimMode::None + 1) : SimMode(simModeInt + 1);

				isKeyDownRight = true;
			} else if(isKeyDownRight && !App::Key(VK_RIGHT)){
				isKeyDownRight = false;
			}

			if(canMakeSimMap && App::IsMousePressed(GLFW_MOUSE_BUTTON_MIDDLE)){
				myThread = new std::thread(&Scene::MakeSimMap, this);
				canMakeSimMap = false;
			}

			break;
		}
		case SimRuntimeStatus::MakingTheMap:
			im_Cam.Update(updateDt);
			if(App::Key('R')){
				orthoProjectionScaleFactor = 1.0;
			}
			orthoProjectionScaleFactor -= mouseScrollWheelYOffset * 0.02; //No need dt
			orthoProjectionScaleFactor = Math::Clamp(orthoProjectionScaleFactor, 0.2, 1.0);

			break;
		case SimRuntimeStatus::Ongoing:
			im_Cam.Update(updateDt);
			if(App::Key('R')){
				orthoProjectionScaleFactor = 1.0;
			}
			orthoProjectionScaleFactor -= mouseScrollWheelYOffset * 0.02; //No need dt
			orthoProjectionScaleFactor = Math::Clamp(orthoProjectionScaleFactor, 0.2, 1.0);

			sim->Update(updateDt); //Not (dt * sim->spd) as...
			UpdateEntities(updateDt * sim->spd);

			break;
	}
	mouseScrollWheelYOffset = 0.0;
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

	static bool isKeyDownO = false;
	static bool isKeyDownP = false;
	if(!isKeyDownO && App::Key('O')){
		shldRenderTileArt = !shldRenderTileArt;
		isKeyDownO = true;
	} else if(isKeyDownO && !App::Key('O')){
		isKeyDownO = false;
	}
	if(!isKeyDownP && App::Key('P')){
		shldRenderFog = !shldRenderFog;
		isKeyDownP = true;
	} else if(isKeyDownP && !App::Key('P')){
		isKeyDownP = false;
	}

	float& simSpd = sim->spd;
	static bool isKeyDownZ = false;
	static bool isKeyDownX = false;
	if(!isKeyDownZ && App::Key('Z')){
		simSpd += 0.1f;
		isKeyDownZ = true;
	} else if(isKeyDownZ && !App::Key('Z')){
		isKeyDownZ = false;
	}
	if(!isKeyDownX && App::Key('X')){
		simSpd -= 0.1f;
		isKeyDownX = true;
	} else if(isKeyDownX && !App::Key('X')){
		isKeyDownX = false;
	}
	simSpd = Math::Clamp(simSpd, 0.2f, 4.0f);

	static bool isKeyDownC = false;
	static bool isKeyDownV = false;
	if(!isKeyDownC && App::Key('C')){
		static SimTurn savedTurn = SimTurn::Amt;
		if(sim->turn != SimTurn::Environment){
			if(Math::RandIntMinMax(1, 10) <= 2){
				savedTurn = sim->turn;
				sim->turn = SimTurn::Environment;
			} else{
				sim->turn = sim->turn == SimTurn::Player ? SimTurn::AI : SimTurn::Player;
			}
		} else{
			sim->turn = savedTurn == SimTurn::Player ? SimTurn::AI : SimTurn::Player;
		}

		isKeyDownC = true;
	} else if(isKeyDownC && !App::Key('C')){
		isKeyDownC = false;
	}
	if(!isKeyDownV && App::Key('V')){
		sim->timeOfDay = (SimTimeOfDay)Math::RandIntMinMax((int)SimTimeOfDay::Day, (int)SimTimeOfDay::Amt - 1);
		isKeyDownV = true;
	} else if(isKeyDownV && !App::Key('V')){
		isKeyDownV = false;
	}

	static bool isKeyDownB = false;
	static bool isKeyDownN = false;
	if(!isKeyDownB && App::Key('B')){
		sim->turnElapsedTime = 0.0f;
		isKeyDownB = true;
	} else if(isKeyDownB && !App::Key('B')){
		isKeyDownB = false;
	}
	if(!isKeyDownN && App::Key('N')){
		sim->timeOfDayElapsedTime = 0.0f;
		isKeyDownN = true;
	} else if(isKeyDownN && !App::Key('N')){
		isKeyDownN = false;
	}

	static bool isKeyDownT = false;
	static bool isKeyDownY = false;
	static bool isKeyDownU = false;
	static bool isKeyDownI = false;
	if(!isKeyDownT && App::Key('T')){
		sim->turnDuration += 0.1f;
		isKeyDownT = true;
	} else if(isKeyDownT && !App::Key('T')){
		isKeyDownT = false;
	}
	if(!isKeyDownY && App::Key('Y')){
		sim->turnDuration -= 0.1f;
		isKeyDownY = true;
	} else if(isKeyDownY && !App::Key('Y')){
		isKeyDownY = false;
	}
	if(!isKeyDownU && App::Key('U')){
		sim->timeOfDayDuration += 0.1f;
		RoundedTo2dp(sim->timeOfDayDuration);
		isKeyDownU = true;
	} else if(isKeyDownU && !App::Key('U')){
		isKeyDownU = false;
	}
	if(!isKeyDownI && App::Key('I')){
		sim->timeOfDayDuration -= 0.1f;
		RoundedTo2dp(sim->timeOfDayDuration);
		isKeyDownI = true;
	} else if(isKeyDownI && !App::Key('I')){
		isKeyDownI = false;
	}
}

void Scene::UpdateEntities(const double dt){
	static bool hasSpawned = false;
	if(!hasSpawned){
		Entity* const entity = entityPool->ActivateObj();
		entity->im_Attribs.im_Type = Obj::EntityType::Knight;
		entity->im_Attribs.im_LocalPos.x = 2.0f;
		entity->im_Attribs.im_LocalPos.y = 2.0f;
		entity->im_Attribs.im_CurrHealth = 5.0f;
		entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;
		hasSpawned = true;
	}

	for(Entity* const& entity: activeEntities){
		switch(entity->im_Attribs.im_Type){
			case Obj::EntityType::Knight:
				break;
			case Obj::EntityType::Gunner:
				break;
			case Obj::EntityType::Healer:
				break;
			case Obj::EntityType::King:
				break;
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
			RenderCoverMap();
			RenderCoverText();
			break;
		case SimRuntimeStatus::Ongoing:
			RenderMap();
			RenderEntities();
			break;
		case SimRuntimeStatus::MakingTheMap:
			RenderMap();
			break;
	}
	RenderSceneText();

	modelStack.PopMatrix();

	if(sim->status == SimRuntimeStatus::Waiting){
		canMakeSimMap = true;
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
	const float gridWidth = grid->CalcWidth();
	const float gridHeight = grid->CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f;

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			modelStack.PushMatrix();

			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Translate(
					xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c,
					yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
					0.05f
				);
				modelStack.Scale(
					gridCellScaleX + gridLineThickness * 2.5f,
					gridCellScaleY + gridLineThickness * 2.5f,
					1.0f
				);
			} else{
				modelStack.Translate(
					xOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)c + (r & 1) * grid->CalcAltOffsetX(),
					yOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)r,
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
					xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c,
					yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
					0.1f
				);
				modelStack.Scale(
					gridCellScaleX,
					gridCellScaleY,
					1.0f
				);
			} else{
				modelStack.Translate(
					xOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)c + (r & 1) * grid->CalcAltOffsetX(),
					yOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)r,
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
		meshList[(int)GeoType::TextMod1],
		"Press Middle Mouse Button (MMB) to start!",
		Color(),
		textSize1,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f + textSize0 + textSize1 * 2.0f,
		TextAlignment::Center
	);
	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod2],
		"Modify hex grid (1 - 8, ~)",
		Color(),
		textSize0,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f + textSize0 * 0.75f,
		TextAlignment::Center
	);
	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod2],
		"Change sim mode (< and/or >)",
		Color(),
		textSize0,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f - textSize0 * 0.75f,
		TextAlignment::Center
	);
	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod1],
		"Press Middle Mouse Button (MMB) to start!",
		Color(),
		textSize2,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f - textSize0 - textSize2 * 2.0f,
		TextAlignment::Center
	);
}

void Scene::RenderEntities(){
	static float individualDepthOffset = 0.0f;

	const float gridWidth = grid->CalcWidth();
	const float gridHeight = grid->CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f;

	for(const Entity* const& entity: activeEntities){
		modelStack.PushMatrix();
		const Vector3 localPos = Vector3(entity->im_Attribs.im_LocalPos.x, entity->im_Attribs.im_LocalPos.y, 0.0f);

		if(gridType == HexGrid<float>::GridType::FlatTop){
			modelStack.Translate(
				xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * localPos.x,
				yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * localPos.y + ((int)localPos.x & 1) * grid->CalcAltOffsetY(),
				0.25f + individualDepthOffset
			);
			modelStack.Scale(
				gridCellScaleX + gridLineThickness * 2.5f,
				gridCellScaleY + gridLineThickness * 2.5f,
				1.0f
			);
		} else{
			modelStack.Translate(
				xOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * localPos.x + ((int)localPos.y & 1) * grid->CalcAltOffsetX(),
				yOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * localPos.y,
				0.25f + individualDepthOffset
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

		RenderMesh(meshList[(int)GeoType::Hex], true, Color(), 1.0f);

			const float ratio = entity->im_Attribs.im_CurrHealth / entity->im_Attribs.im_MaxHealth;
			modelStack.PushMatrix();

			modelStack.Translate(
				0.0f,
				0.3f,
				0.25f + individualDepthOffset
			);
			modelStack.Scale(
				0.9f,
				0.05f,
				1.0f
			);

			if(entity->im_Attribs.im_CurrHealth > 0.0f){
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

				RenderMesh(meshList[(int)GeoType::Quad], true, Color(ratio < 0.5f ? 1.0f : (1.0f - ratio) * 2.0f, ratio > 0.5f ? 1.0f : ratio * 2.0f, 0.0f), 1.0f);
				modelStack.PopMatrix();

				RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.1f, 0.1f, 0.1f), 1.0f);
			}

			modelStack.PopMatrix();

		modelStack.PopMatrix();

		individualDepthOffset += 0.01f;
	}

	individualDepthOffset = 0.0f;
}

void Scene::RenderMap(){
	const float gridWidth = grid->CalcWidth();
	const float gridHeight = grid->CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f;

	const std::vector<TileType>& tileLayer = sim->GetTileLayer();
	const int tileLayerSize = (int)tileLayer.size();

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			modelStack.PushMatrix();

			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Translate(
					xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c,
					yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
					0.05f
				);
				modelStack.Scale(
					gridCellScaleX + gridLineThickness * 2.5f,
					gridCellScaleY + gridLineThickness * 2.5f,
					1.0f
				);
			} else{
				modelStack.Translate(
					xOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)c + (r & 1) * grid->CalcAltOffsetX(),
					yOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)r,
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

			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 0.0f, 0.0f), 0.7f);

			modelStack.PopMatrix();

			if(r * gridCols + c >= tileLayerSize){
				continue;
			}

			modelStack.PushMatrix();

			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Translate(
					xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c,
					yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
					0.1f
				);
				modelStack.Scale(
					gridCellScaleX,
					gridCellScaleY,
					1.0f
				);
			} else{
				modelStack.Translate(
					xOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)c + (r & 1) * grid->CalcAltOffsetX(),
					yOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)r,
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

	if(shldRenderFog){
		glDepthFunc(GL_ALWAYS);

		const std::vector<FogType>& fogLayer = sim->GetFogLayer();
		const int fogLayerSize = (int)fogLayer.size();

		for(int r = 0; r < gridRows; ++r){
			for(int c = 0; c < gridCols; ++c){
				if(r * gridCols + c >= fogLayerSize){
					continue;
				}

				modelStack.PushMatrix();

				if(gridType == HexGrid<float>::GridType::FlatTop){
					modelStack.Translate(
						xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c,
						yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
						0.1f
					);
				} else{
					modelStack.Translate(
						xOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)c + (r & 1) * grid->CalcAltOffsetX(),
						yOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)r,
						0.1f
					);
				}
				modelStack.Scale(
					gridCellScaleX,
					gridCellScaleY,
					1.0f
				);

				RenderFog(fogLayer, r, c);

				modelStack.PopMatrix();
			}
		}

		glDepthFunc(GL_LESS);
	}
}

void Scene::RenderFog(const std::vector<FogType>& fogLayer, const int r, const int c){
	switch(fogLayer[r * gridCols + c]){
		case FogType::Thin:
			modelStack.PushMatrix();

			modelStack.Translate(
				0.0f,
				0.0f,
				0.1f
			);
			modelStack.Rotate(
				elapsedTime * 160.0f,
				0.0f,
				0.0f,
				1.0f
			);
			modelStack.Scale(
				0.9f,
				0.9f,
				0.9f
			);

			RenderMesh(meshList[(int)GeoType::Fog], true, Color(1.0f, 0.63f, 0.63f), 0.8f);

			modelStack.PopMatrix();
			break;
		case FogType::Thick:
			modelStack.PushMatrix();

			modelStack.Translate(
				0.0f,
				0.0f,
				0.1f
			);
			modelStack.Rotate(
				elapsedTime * -80.0f,
				0.0f,
				0.0f,
				1.0f
			);

			RenderMesh(meshList[(int)GeoType::Fog], true, Color(0.63f, 0.63f, 1.0f), 0.6f);

			modelStack.PopMatrix();
			break;
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
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.8f, 0.52f, 0.25f), 1.0f);

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

				RenderMesh(meshList[(int)GeoType::SoilTile], true, Color(), 1.0f);

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

				RenderMesh(meshList[(int)GeoType::WaterTile], true, Color(), 1.0f);

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
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.4f, 0.26f, 0.13f), 1.0f);

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

				RenderMesh(meshList[(int)GeoType::MudTile], true, Color(), 1.0f);

				modelStack.PopMatrix();
			}
			break;
	}
}

void Scene::RenderSceneText(){
	static Mesh* const textMesh = meshList[(int)GeoType::Text];
	static float textSize = (float)windowWidth * 0.02f;

	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod1],
		"AI's Credits: " + std::to_string(creditsAI),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		(float)windowHeight - textSize * 2.0f,
		TextAlignment::Center
	);
	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod2],
		"Your Credits: " + std::to_string(creditsPlayer),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		textSize,
		TextAlignment::Center
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
		textSize * 0.0f
	);
	RenderTextOnScreen(
		textMesh,
		"Update FPS: " + std::to_string(updateFPS).substr(0, std::to_string((int)updateFPS).length() + 3),
		textColor,
		textSize,
		0.0f,
		textSize * 1.0f
	);
	RenderTextOnScreen(
		textMesh,
		"Render FPS: " + std::to_string(renderFPS).substr(0, std::to_string((int)renderFPS).length() + 3),
		textColor,
		textSize,
		0.0f,
		textSize * 2.0f
	);
}

void Scene::RenderControlsText(Mesh* const textMesh, const Color& textColor, const float textSize){
	static std::string texts[]{
		"F1: Toggle fullscreen",
		"F2: Change polygon mode",
		"F3: Toggle visibility of app window",
		"WASD: Move cam",
		"R: Reset cam",
		"O: Toggle tile art",
		"P: Toggle fog",
		"Z: Increase sim spd",
		"X: Decrease sim spd",
		"C: Change sim turn manually",
		"V: Change sim time of day manually",
		"B: Reset sim turn elapsed time",
		"N: Reset sim time of day elapsed time",
		"T: Increase sim turn duration",
		"Y: Decrease sim turn duration",
		"U: Increase sim time of day duration",
		"I: Decrease sim time of day duration",
	};
	static size_t size = sizeof(texts) / sizeof(texts[0]);

	for(size_t i = 0; i < size; ++i){
		RenderTextOnScreen(
			textMesh,
			texts[i],
			textColor,
			textSize,
			0.0f,
			(float)windowHeight - textSize * float(i + 1)
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
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Grid cell scale y: " + std::to_string(gridCellScaleY).substr(0, std::to_string((int)gridCellScaleY).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 5.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Grid line thickness: " + std::to_string(gridLineThickness).substr(0, std::to_string((int)gridLineThickness).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 4.0f,
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
		textSize * 2.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		(std::string)"shldRenderTileArt: " + (shldRenderTileArt ? "Yes" : "No"),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 1.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		(std::string)"shldRenderFog: " + (shldRenderFog ? "Yes" : "No"),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 0.0f,
		TextAlignment::Right
	);
}

void Scene::RenderSimInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
	static std::string runtimeStatusTexts[(int)SimRuntimeStatus::Amt]{
		"is waiting",
		"is making the map",
		"is ongoing",
		"has ended",
	};
	static std::string modeTexts[(int)SimMode::Amt]{
		"None",
		"Protect The King!",
		"-- Last Team Standing --",
		"~ Greatest Area Painted ~"
	};
	static std::string turnTexts[(int)SimTurn::Amt]{
		"Nil",
		"Player",
		"AI",
		"Environment"
	};
	static std::string timeOfDayTexts[(int)SimTimeOfDay::Amt]{
		"Day",
		"Rainy",
		"Night",
	};

	const std::string texts[]{
		(std::string)"Sim " + runtimeStatusTexts[(int)sim->status],
		"Sim mode: " + modeTexts[(int)sim->mode],
		"Sim spd: " + std::to_string(sim->spd).substr(0, std::to_string((int)sim->spd).length() + 3),
		"Sim turn duration: " + std::to_string(sim->turnDuration).substr(0, std::to_string((int)sim->turnDuration).length() + 3),
		"Sim turn elapsed time: " + std::to_string(sim->turnElapsedTime).substr(0, std::to_string((int)sim->turnElapsedTime).length() + 3),
		"Sim turn: " + turnTexts[(int)sim->turn],
		"Sim time of day duration: " + std::to_string(sim->timeOfDayDuration).substr(0, std::to_string((int)sim->timeOfDayDuration).length() + 3),
		"Sim time of day elapsed time: " + std::to_string(sim->timeOfDayElapsedTime).substr(0, std::to_string((int)sim->timeOfDayElapsedTime).length() + 3),
		"Sim time of day: " + timeOfDayTexts[(int)sim->timeOfDay],
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
			TextAlignment::Right
		);
	}
}

void Scene::MakeSimMap() const{
	sim->status = SimRuntimeStatus::MakingTheMap;

	sim->ChangeFogWeight((int)FogType::Inexistent, 5);
	sim->ChangeFogWeight((int)FogType::Thin, 20);
	sim->ChangeFogWeight((int)FogType::Thick, 20);

	sim->ChangeTileWeight((int)TileType::Invalid, 0);
	sim->ChangeTileWeight((int)TileType::Wall, 70);
	sim->ChangeTileWeight((int)TileType::Empty, 10);
	sim->ChangeTileWeight((int)TileType::Soil, 30);
	sim->ChangeTileWeight((int)TileType::Fire, 20);
	sim->ChangeTileWeight((int)TileType::Water, 20);
	sim->ChangeTileWeight((int)TileType::Grass, 20);
	sim->ChangeTileWeight((int)TileType::Mud, 20);

	const bool isFlatTop = gridType == HexGrid<float>::GridType::FlatTop;
	const float* const quickRenderDelay0 = new float(0.05f);
	const float* const quickRenderDelay1 = new float(0.02f);
	const float* const quickRenderDelay2 = new float(0.04f);
	const float* const quickRenderDelay3 = new float(0.01f);
	sim->GenTileLayer(gridRows, gridCols, 0, 0, 2169, quickRenderDelay0, isFlatTop);
	sim->RefineTileLayer(gridRows, gridCols, 2169, quickRenderDelay1);
	sim->MakeRadialHoleInTileLayer(gridRows, gridCols, 14, 4, 1, quickRenderDelay2, isFlatTop);
	sim->MakeRadialHoleInTileLayer(gridRows, gridCols, 4, 5, 2, quickRenderDelay2, isFlatTop);
	sim->MakeRadialHoleInTileLayer(gridRows, gridCols, 10, 12, 3, quickRenderDelay2, isFlatTop);
	sim->GenFogLayer(gridRows, gridCols, 0, 0, 2169, quickRenderDelay3);
	delete quickRenderDelay0;
	delete quickRenderDelay1;
	delete quickRenderDelay2;
	delete quickRenderDelay3;

	sim->status = SimRuntimeStatus::Ongoing;
}