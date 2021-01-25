#include "Scene.h"

#include "App.h"
#include "GLFW/glfw3.h"

#include "Easing.hpp"

#include "EventAddCredits.h"
#include "ListenerFlags.hpp"

#include "KnightCreationAttribs.h"

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
	spawnCostPlayer(10),
	spawnCostAI(10),
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
	gridWidth(0.0f),
	gridHeight(0.0f),
	gridOffsetX(0.0f),
	gridOffsetY(0.0f),
	mouseRow(0.0f),
	mouseCol(0.0f),
	gridCellFlatToFlatLen(0.0f),
	gridCellSideLen(0.0f),
	gridAltOffsetX(0.0f),
	gridAltOffsetY(0.0f),
	sim(new Sim()),
	grid(new HexGrid<float>(HexGrid<float>::GridType::Amt, 0.0f, 0.0f, 0.0f, 0, 0)),
	entityFactory(Obj::EntityFactory<Vector3, float>::RetrieveGlobalObjPtr()),
	entityPool(Obj::ObjPool<Entity>::RetrieveGlobalObjPtr()),
	publisher(Publisher::RetrieveGlobalObjPtr()),
	activeEntities(entityPool->GetActiveObjs()),
	entitiesToDeactivate(),
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
	sim->turnDuration = 10.0f;
	sim->turnElapsedTime = 0.0f;
	sim->turn = (bool)Math::RandIntMinMax(0, 1) ? SimTurn::Player : SimTurn::AI;
	sim->timeOfDayDuration = 20.0f;
	sim->timeOfDayElapsedTime = 0.0f;
	sim->timeOfDay = (SimTimeOfDay)Math::RandIntMinMax((int)SimTimeOfDay::Day, (int)SimTimeOfDay::Amt - 1);

	grid->SetGridType(gridType);
	grid->SetCellScaleX(gridCellScaleX);
	grid->SetCellScaleY(gridCellScaleY);
	grid->SetLineThickness(gridLineThickness);
	grid->SetRows(gridRows);
	grid->SetCols(gridCols);

	publisher->AddListener((long int)ListenerFlags::Scene, this);
}

void Scene::Update(const double updateDt, const double renderDt){
	SceneSupport::Update(updateDt, renderDt);

	OnEvent(FetchEvent(), true);
	UpdateMisc(updateDt);

	switch(sim->status){
		case SimRuntimeStatus::Waiting: {
			UpdateGridAttribs();

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

			static bool isKeyDownSpace = false;
			if(!isKeyDownSpace && App::Key(VK_SPACE)){
				if(sim->turn == SimTurn::Player){
					sim->turn = Math::RandIntMinMax(1, 10) <= 2 ? SimTurn::Environment : SimTurn::AI;
					sim->turnElapsedTime = 0.0f;

					if(sim->turn == SimTurn::Player){
						creditsPlayer += 100;
					} else if(sim->turn == SimTurn::AI){
						creditsAI += 100;
					}
				}
				isKeyDownSpace = true;
			} else if(isKeyDownSpace && !App::Key(VK_SPACE)){
				isKeyDownSpace = false;
			}

			static bool isKeyDownQ = false;
			if(!isKeyDownQ && App::Key('Q')){
				entityFactory->SpawnRandUnit(Obj::KnightCreationAttribs<Vector3, float>());

				isKeyDownQ = true;
			} else if(isKeyDownQ && !App::Key('Q')){
				isKeyDownQ = false;
			}

			sim->Update(updateDt); //Not (dt * sim->spd) as...
			UpdateEntities(updateDt * sim->spd);

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

	gridCellFlatToFlatLen = grid->CalcCellFlatToFlatLen();
	gridCellSideLen = grid->CalcCellSideLen();
	gridAltOffsetX = grid->CalcAltOffsetX();
	gridAltOffsetY = grid->CalcAltOffsetY();

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
	for(Entity* const& entity: activeEntities){
		switch(entity->im_Attribs.im_Type){
			case Obj::EntityType::Knight:
				if(entity->im_Attribs.im_CurrHealth <= 0.0f){
					entitiesToDeactivate.emplace_back(entity);
				}

				break;
			case Obj::EntityType::Gunner:
				if(entity->im_Attribs.im_CurrHealth <= 0.0f){
					entitiesToDeactivate.emplace_back(entity);
				}

				break;
			case Obj::EntityType::Healer:
				if(entity->im_Attribs.im_CurrHealth <= 0.0f){
					entitiesToDeactivate.emplace_back(entity);
				}

				break;
		}
	}

	for(Entity* const entity: entitiesToDeactivate){
		entityPool->DeactivateObj(entity);
	}
	entitiesToDeactivate.clear();
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
			if(sim->turn == SimTurn::Player){
				RenderGridCellOfMouse();
			}
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

	for(const Entity* const entity: activeEntities){
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

		RenderMesh(meshList[(int)GeoType::Circle], true, entity->im_Attribs.im_Team == Obj::EntityTeam::AI ? Color(0.4f, 0.0f, 0.0f) : Color(0.0f, 0.4f, 0.0f), 1.0f);
		RenderEntityArt(entity);
		RenderHealthBar(entity);
		RenderEntityLvl(entity);

		modelStack.PopMatrix();

		individualDepthOffset += 0.01f;
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
	switch(entity->im_Attribs.im_FacingDir){
		case Obj::EntityFacingDir::Up:
			if(gridType == HexGrid<float>::GridType::SharpTop){
				if((int)entity->im_Attribs.im_LocalPos.y & 1){
					modelStack.Rotate(
						30.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						330.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			}
			break;
		case Obj::EntityFacingDir::Down:
			if(gridType == HexGrid<float>::GridType::FlatTop){
				modelStack.Rotate(
					180.0f,
					0.0f,
					0.0f,
					1.0f
				);
			} else{
				if((int)entity->im_Attribs.im_LocalPos.y & 1){
					modelStack.Rotate(
						150.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						210.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			}
			break;
		case Obj::EntityFacingDir::Left:
			if(gridType == HexGrid<float>::GridType::FlatTop){
				if((int)entity->im_Attribs.im_LocalPos.x & 1){
					modelStack.Rotate(
						120.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						60.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			} else{
				modelStack.Rotate(
					90.0f,
					0.0f,
					0.0f,
					1.0f
				);
			}
			break;
		case Obj::EntityFacingDir::Right:
			if(gridType == HexGrid<float>::GridType::FlatTop){
				if((int)entity->im_Attribs.im_LocalPos.x & 1){
					modelStack.Rotate(
						240.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						300.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			} else{
				modelStack.Rotate(
					270.0f,
					0.0f,
					0.0f,
					1.0f
				);
			}
			break;
		case Obj::EntityFacingDir::UL:
			if(gridType == HexGrid<float>::GridType::FlatTop){
				if((int)entity->im_Attribs.im_LocalPos.x & 1){
					modelStack.Rotate(
						60.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						120.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			} else{
				if((int)entity->im_Attribs.im_LocalPos.y & 1){
					modelStack.Rotate(
						330.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						30.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			}
			break;
		case Obj::EntityFacingDir::UR:
			if(gridType == HexGrid<float>::GridType::FlatTop){
				if((int)entity->im_Attribs.im_LocalPos.x & 1){
					modelStack.Rotate(
						300.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						240.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			} else{
				if((int)entity->im_Attribs.im_LocalPos.y & 1){
					modelStack.Rotate(
						330.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						30.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			}
			break;
		case Obj::EntityFacingDir::DL:
			if(gridType == HexGrid<float>::GridType::FlatTop){
				if((int)entity->im_Attribs.im_LocalPos.x & 1){
					modelStack.Rotate(
						60.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						120.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			} else{
				if((int)entity->im_Attribs.im_LocalPos.y & 1){
					modelStack.Rotate(
						210.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						150.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			}
			break;
		case Obj::EntityFacingDir::DR:
			if(gridType == HexGrid<float>::GridType::FlatTop){
				if((int)entity->im_Attribs.im_LocalPos.x & 1){
					modelStack.Rotate(
						300.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						240.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			} else{
				if((int)entity->im_Attribs.im_LocalPos.y & 1){
					modelStack.Rotate(
						210.0f,
						0.0f,
						0.0f,
						1.0f
					);
				} else{
					modelStack.Rotate(
						150.0f,
						0.0f,
						0.0f,
						1.0f
					);
				}
			}
			break;
	}
	modelStack.Scale(
		0.6f,
		0.6f,
		1.0f
	);

	switch(entity->im_Attribs.im_Type){
		case Obj::EntityType::Knight:
			RenderMesh(meshList[(int)GeoType::Knight], false);
			break;
		case Obj::EntityType::Gunner:
			RenderMesh(meshList[(int)GeoType::Gunner], false);
			break;
		case Obj::EntityType::Healer:
			RenderMesh(meshList[(int)GeoType::Healer], false);
			break;
	}

	modelStack.PopMatrix();
}

void Scene::RenderHealthBar(const Entity* const entity){
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

	if(entity->im_Attribs.im_CurrHealth > 0.0f){
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
	}

	RenderMesh(meshList[(int)GeoType::Quad], true, Color(0.1f, 0.1f, 0.1f), 1.0f);

	modelStack.PopMatrix();
}

void Scene::RenderEntityLvl(const Entity* const entity){
	modelStack.PushMatrix();

	modelStack.Translate(
		0.0f,
		-0.4f,
		0.1f
	);
	modelStack.Scale(
		0.4f,
		0.4f,
		1.0f
	);

	RenderText(
		meshList[(int)GeoType::TextMod],
		"Lvl " + std::to_string(entity->im_Attribs.im_Lvl),
		Color(),
		TextAlignment::Center,
		-0.1f
	);

	modelStack.PopMatrix();
}

void Scene::RenderMap(){
	const std::vector<TileType>& tileLayer = sim->GetTileLayer();
	const int tileLayerSize = (int)tileLayer.size();

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

			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 0.0f, 0.0f), 0.7f);

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

	if(shldRenderFog){
		//??
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
				gridOffsetX + (gridCellSideLen * 1.5f + gridLineThickness) * (float)mouseCol,
				gridOffsetY + (gridCellFlatToFlatLen + gridLineThickness) * (float)mouseRow + ((int)mouseCol & 1) * gridAltOffsetY,
				0.4f
			);
			modelStack.Scale(
				gridCellScaleX + gridLineThickness * 2.5f,
				gridCellScaleY + gridLineThickness * 2.5f,
				1.0f
			);
		} else{
			modelStack.Translate(
				gridOffsetX + (gridCellFlatToFlatLen + gridLineThickness) * (float)mouseCol + ((int)mouseRow & 1) * gridAltOffsetX,
				gridOffsetY + (gridCellSideLen * 1.5f + gridLineThickness) * (float)mouseRow,
				0.4f
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

		RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 1.0f, 0.0f), 0.4f);

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
		"AI's Spawn Cost: " + std::to_string(spawnCostAI),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		(float)windowHeight - textSize * 2.0f,
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
		"Your Spawn Cost: " + std::to_string(spawnCostPlayer),
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		textSize,
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
		"Sim time of day elapsed time: " + std::to_string(sim->timeOfDayElapsedTime).substr(0, std::to_string((int)sim->timeOfDayElapsedTime).length() + 3),
		"Sim time of day duration: " + std::to_string(sim->timeOfDayDuration).substr(0, std::to_string((int)sim->timeOfDayDuration).length() + 3),
		"Sim turn elapsed time: " + std::to_string(sim->turnElapsedTime).substr(0, std::to_string((int)sim->turnElapsedTime).length() + 3),
		"Sim turn duration: " + std::to_string(sim->turnDuration).substr(0, std::to_string((int)sim->turnDuration).length() + 3),
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
	const float* const quickRenderDelay0 = new float(0.05f);
	const float* const quickRenderDelay1 = new float(0.02f);
	const float* const quickRenderDelay2 = new float(0.04f);
	sim->GenTileLayer(gridRows, gridCols, 0, 0, 2169, quickRenderDelay0, isFlatTop);
	sim->RefineTileLayer(gridRows, gridCols, 2169, quickRenderDelay1);
	sim->MakeRadialHoleInTileLayer(gridRows, gridCols, 14, 4, 1, quickRenderDelay2, isFlatTop);
	sim->MakeRadialHoleInTileLayer(gridRows, gridCols, 4, 5, 2, quickRenderDelay2, isFlatTop);
	sim->MakeRadialHoleInTileLayer(gridRows, gridCols, 10, 12, 3, quickRenderDelay2, isFlatTop);
	delete quickRenderDelay0;
	delete quickRenderDelay1;
	delete quickRenderDelay2;

	sim->status = SimRuntimeStatus::Ongoing;

	if(sim->turn == SimTurn::Player){
		creditsPlayer += 100;
	} else if(sim->turn == SimTurn::AI){
		creditsAI += 100;
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
	}

	if(destroyEvent && myEvent){
		delete myEvent;
		myEvent = nullptr;
	}
	return val;
}