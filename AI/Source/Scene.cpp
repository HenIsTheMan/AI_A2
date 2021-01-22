#include "Scene.h"

#include "App.h"
#include "GLFW/glfw3.h"

#include "Easing.hpp"

extern int windowWidth;
extern int windowHeight;

Scene::Scene():
	SceneSupport(),
	canMakeSimMap(false),
	shldRenderTileArt(true),
	shldRenderFog(true),
	customHue(0.0f),
	gridType(HexGrid<float>::GridType::FlatTop),
	gridCellScaleX(50.0f),
	gridCellScaleY(50.0f),
	gridLineThickness(4.0f),
	gridRows(19),
	gridCols(19),
	sim(new Sim()),
	grid(new HexGrid<float>(HexGrid<float>::GridType::Amt, 0.0f, 0.0f, 0.0f, 0, 0)),
	publisher(Publisher::RetrieveGlobalObjPtr())
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

	if(publisher){
		publisher->Destroy();
		publisher = nullptr;
	}
}

void Scene::Init(){
	SceneSupport::Init();

	sim->status = RuntimeStatus::Waiting;
	sim->spd = 1.0f;
	sim->turnDuration = 5.0f;
	sim->turnElapsedTime = 0.0f;
	sim->turn = 0;
	sim->timeOfDayDuration = 10.0f;
	sim->timeOfDayElapsedTime = 0.0f;
	sim->timeOfDay = (TimeOfDay)Math::RandIntMinMax((int)TimeOfDay::Day, (int)TimeOfDay::Amt - 1);

	grid->SetGridType(gridType);
	grid->SetCellScaleX(gridCellScaleX);
	grid->SetCellScaleY(gridCellScaleY);
	grid->SetLineThickness(gridLineThickness);
	grid->SetRows(gridRows);
	grid->SetCols(gridCols);
}

void Scene::Update(double dt){
	SceneSupport::Update(dt);

	UpdateMisc(dt);

	switch(sim->status){
		case RuntimeStatus::Waiting:
			if(App::IsMousePressed(GLFW_MOUSE_BUTTON_MIDDLE)){
				sim->status = RuntimeStatus::Ongoing;

				if(canMakeSimMap){
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

					const float* const quickRenderDelay0 = nullptr;
					const float* const quickRenderDelay1 = nullptr;
					const float* const quickRenderDelay2 = nullptr;
					const float* const quickRenderDelay3 = nullptr;
					sim->GenTileLayer(gridRows, gridCols, 0, 0, 2169, quickRenderDelay0);
					sim->RefineTileLayer(gridRows, gridCols, 2169, quickRenderDelay1);
					sim->MakeRadialHoleInTileLayer(gridRows, gridCols, 5, 5, 2, quickRenderDelay2);
					sim->GenFogLayer(gridRows, gridCols, 0, 0, 2169, quickRenderDelay3);
					delete quickRenderDelay0;
					delete quickRenderDelay1;
					delete quickRenderDelay2;
					delete quickRenderDelay3;

					canMakeSimMap = false;
				}
			}
			break;
		case RuntimeStatus::Ongoing:
			sim->Update(dt); //Not (dt * sim->spd) as...
			UpdateEntities(dt * sim->spd);
			break;
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
		++sim->turn;
		isKeyDownC = true;
	} else if(isKeyDownC && !App::Key('C')){
		isKeyDownC = false;
	}
	if(!isKeyDownV && App::Key('V')){
		sim->timeOfDay = (TimeOfDay)Math::RandIntMinMax((int)TimeOfDay::Day, (int)TimeOfDay::Amt - 1);
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
		isKeyDownU = true;
	} else if(isKeyDownU && !App::Key('U')){
		isKeyDownU = false;
	}
	if(!isKeyDownI && App::Key('I')){
		sim->timeOfDayDuration -= 0.1f;
		isKeyDownI = true;
	} else if(isKeyDownI && !App::Key('I')){
		isKeyDownI = false;
	}
}

void Scene::UpdateEntities(const double dt){
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
		case RuntimeStatus::Waiting:
			RenderCoverMap();
			RenderCoverText();
			break;
		case RuntimeStatus::Ongoing:
			RenderMap();
			RenderEntities();
			break;
	}
	RenderSceneText();

	modelStack.PopMatrix();

	if(sim->status == RuntimeStatus::Waiting){
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
		case TimeOfDay::Day:
			RenderMesh(meshList[(int)GeoType::DayBG], false);
			break;
		case TimeOfDay::Rainy:
			RenderMesh(meshList[(int)GeoType::RainyBG], false);
			break;
		case TimeOfDay::Night:
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

	const std::vector<FogType>& fogLayer = sim->GetFogLayer();
	const std::vector<TileType>& tileLayer = sim->GetTileLayer();

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			modelStack.PushMatrix();

			modelStack.Translate(
				xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c + (c & 1) * grid->CalcAltOffsetX(),
				yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
				0.05f
			);
			modelStack.Scale(
				gridCellScaleX + gridLineThickness * 2.5f,
				gridCellScaleY + gridLineThickness * 2.5f,
				1.0f
			);

			RenderMesh(meshList[(int)GeoType::Hex], true, Color::HSVToRGB({customHue, 1.0f, 1.0f}), 0.5f);

			modelStack.PopMatrix();

			modelStack.PushMatrix();

			modelStack.Translate(
				xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c + (c & 1) * grid->CalcAltOffsetX(),
				yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
				0.1f
			);
			modelStack.Scale(
				gridCellScaleX,
				gridCellScaleY,
				1.0f
			);

			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 0.0f, 0.0f), 0.5f);

			modelStack.PopMatrix();
		}
	}
}

void Scene::RenderCoverText(){
	static float startTextSize = (float)windowWidth * 0.02f;
	static float endTextSize = (float)windowWidth * 0.03f;
	const float lerpFactor = EaseInOutSine(sin(elapsedTime * 7.0f) * 0.5f + 0.5f);
	const float textSize = (1.0f - lerpFactor) * startTextSize + lerpFactor * endTextSize;

	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod1],
		"Press Middle Mouse Button (MMB) to start!",
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f,
		TextAlignment::Center
	);
	RenderTextOnScreen(
		meshList[(int)GeoType::TextMod2],
		"Modify the grid now if u want :)",
		Color(),
		textSize,
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.5f - textSize,
		TextAlignment::Center
	);
}

void Scene::RenderEntities(){
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

			modelStack.Translate(
				xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c + (c & 1) * grid->CalcAltOffsetX(),
				yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
				0.05f
			);
			modelStack.Scale(
				gridCellScaleX + gridLineThickness * 2.5f,
				gridCellScaleY + gridLineThickness * 2.5f,
				1.0f
			);

			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 0.0f, 0.0f), 0.7f);

			modelStack.PopMatrix();

			if(r * gridCols + c >= tileLayerSize){
				continue;
			}

			modelStack.PushMatrix();

			modelStack.Translate(
				xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c + (c & 1) * grid->CalcAltOffsetX(),
				yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
				0.1f
			);
			modelStack.Scale(
				gridCellScaleX,
				gridCellScaleY,
				1.0f
			);

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

				modelStack.Translate(
					xOffset + (grid->CalcCellSideLen() * 1.5f + gridLineThickness) * (float)c + (c & 1) * grid->CalcAltOffsetX(),
					yOffset + (grid->CalcCellFlatToFlatLen() + gridLineThickness) * (float)r + (c & 1) * grid->CalcAltOffsetY(),
					0.1f
				);
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
				elapsedTime * 120.0f,
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
				elapsedTime * -70.0f,
				0.0f,
				0.0f,
				1.0f
			);
			modelStack.Scale(
				0.9f,
				0.9f,
				0.9f
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
				modelStack.Scale(
					0.7f,
					0.7f,
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

	RenderDebugInfoText(textMesh, Color(0.0f, 1.0f, 0.0f), textSize);
	RenderControlsText(textMesh, Color(1.0f, 0.0f, 1.0f), textSize);
	RenderGridAttribsText(textMesh, Color(1.0f, 1.0f, 0.0f), textSize);
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
		"FPS: " + std::to_string(FPS).substr(0, std::to_string((int)FPS).length() + 3),
		textColor,
		textSize,
		0.0f,
		textSize * 1.0f
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
		"C: Increment sim turn manually",
		"V: Change sim time of day manually",
		"B: Reset sim turn elapsed time",
		"N: Reset sim time of day elapsed time",
		"T: Increase sim turn duration",
		"Y: Decrease sim turn duration",
		"U: Increase sim time of day duration",
		"I: Decrease sim time of day duration",
	};
	static size_t size = sizeof(texts) / sizeof(texts[0]);
	static float val = 25.5f;

	for(size_t i = 0; i < size; ++i){
		RenderTextOnScreen(
			textMesh,
			texts[i],
			textColor,
			textSize,
			0.0f,
			textSize * (val - (float)i)
		);
	}
}

void Scene::RenderGridAttribsText(Mesh* const textMesh, const Color& textColor, const float textSize){
	RenderTextOnScreen(
		textMesh,
		"Grid cell scale x: " + std::to_string(gridCellScaleX).substr(0, std::to_string((int)gridCellScaleX).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 0.0f,
		TextAlignment::Right
	);
	RenderTextOnScreen(
		textMesh,
		"Grid cell scale y: " + std::to_string(gridCellScaleY).substr(0, std::to_string((int)gridCellScaleY).length() + 2),
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

void Scene::RenderSimInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
	static std::string runtimeStatusTexts[(int)RuntimeStatus::Amt]{
		"is waiting",
		"is ongoing",
		"has ended",
	};
	static std::string timeOfDayTexts[(int)TimeOfDay::Amt]{
		"Day",
		"Rainy",
		"Night",
	};

	const std::string texts[]{
		(std::string)"Sim " + runtimeStatusTexts[(int)sim->status],
		"Sim spd: " + std::to_string(sim->spd).substr(0, std::to_string((int)sim->spd).length() + 3),
		"Sim turn duration: " + std::to_string(sim->turnDuration).substr(0, std::to_string((int)sim->turnDuration).length() + 3),
		"Sim turn elapsed time: " + std::to_string(sim->turnElapsedTime).substr(0, std::to_string((int)sim->turnElapsedTime).length() + 3),
		"Sim turn: " + std::to_string(sim->turn),
		"Sim time of day duration: " + std::to_string(sim->timeOfDayDuration).substr(0, std::to_string((int)sim->timeOfDayDuration).length() + 3),
		"Sim time of day elapsed time: " + std::to_string(sim->timeOfDayElapsedTime).substr(0, std::to_string((int)sim->timeOfDayElapsedTime).length() + 3),
		"Sim time of day: " + timeOfDayTexts[(int)sim->timeOfDay],
		(std::string)"shldRenderTileArt: " + (shldRenderTileArt ? "Yes" : "No"),
		(std::string)"shldRenderFog: " + (shldRenderFog ? "Yes" : "No"),
	};
	const size_t size = sizeof(texts) / sizeof(texts[0]);
	static float val = 25.5f;

	for(size_t i = 0; i < size; ++i){
		RenderTextOnScreen(
			textMesh,
			texts[i],
			textColor,
			textSize,
			(float)windowWidth,
			textSize * (val - (float)i),
			TextAlignment::Right
		);
	}
}