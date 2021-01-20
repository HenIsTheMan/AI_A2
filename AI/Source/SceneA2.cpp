#include "SceneA2.h"

#include "App.h"

extern int windowWidth;
extern int windowHeight;

SceneA2::SceneA2():
	SceneSupport(),
	gridType(HexGrid<float>::GridType::FlatTop),
	gridCellScaleX(50.0f),
	gridCellScaleY(50.0f),
	gridLineThickness(5.0f),
	gridRows(19),
	gridCols(19),
	sim(new Sim()),
	grid(new HexGrid<float>(HexGrid<float>::GridType::Amt, 0.0f, 0.0f, 0.0f, 0, 0)),
	objPool(new ObjPool<Entity>()),
	publisher(Publisher::RetrieveGlobalObjPtr())
{
	sim->ChangeWeight((int)TileType::Invalid, 0);
	sim->ChangeWeight((int)TileType::Wall, 70);
	sim->ChangeWeight((int)TileType::Empty, 10);
	sim->ChangeWeight((int)TileType::Soil, 30);
	sim->ChangeWeight((int)TileType::Fire, 20);
	sim->ChangeWeight((int)TileType::Water, 20);
	sim->ChangeWeight((int)TileType::Grass, 20);
	sim->ChangeWeight((int)TileType::Mud, 20);
	sim->SetTimeOfDay(TimeOfDay::Rainy);
	sim->Start(gridRows, gridCols, 0, 0, 2169);

	grid->SetGridType(gridType);
	grid->SetCellScaleX(gridCellScaleX);
	grid->SetCellScaleY(gridCellScaleY);
	grid->SetLineThickness(gridLineThickness);
	grid->SetRows(gridRows);
	grid->SetCols(gridCols);
}

SceneA2::~SceneA2(){
	if(sim != nullptr){
		delete sim;
		sim = nullptr;
	}

	if(grid != nullptr){
		delete grid;
		grid = nullptr;
	}

	if(objPool){
		delete objPool;
		objPool = nullptr;
	}

	if(publisher){
		publisher->Destroy();
		publisher = nullptr;
	}
}

void SceneA2::Update(double dt){
	SceneSupport::Update(dt);

	float& gameSpd = sim->RetrieveGameSpd();
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

	UpdateEntities(dt * gameSpd);
}

void SceneA2::UpdateEntities(const double dt){
}

void SceneA2::Render(){
	SceneSupport::Render();

	modelStack.PushMatrix();

	modelStack.Translate(
		-(float)windowWidth * 0.5f,
		-(float)windowHeight * 0.5f,
		0.0f
	);

	RenderBG();
	RenderMap();
	RenderEntities();
	RenderSceneText();

	modelStack.PopMatrix();
}

void SceneA2::RenderBG(){
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

	switch(sim->GetTimeOfDay()){
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

void SceneA2::RenderEntities(){
}

void SceneA2::RenderMap(){
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

			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 0.0f, 0.0f), 0.7f);

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

			RenderFog(fogLayer, r, c);
			RenderTile(tileLayer, r, c);

			modelStack.PopMatrix();
		}
	}
}

void SceneA2::RenderFog(const std::vector<FogType>& fogLayer, const int r, const int c){
	switch(fogLayer[r * gridCols + c]){
		case FogType::Thin:
			break;
		case FogType::Thick:
			break;
	}
}

void SceneA2::RenderTile(const std::vector<TileType>& tileLayer, const int r, const int c){
	switch(tileLayer[r * gridCols + c]){
		case TileType::Empty:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.7f, 0.7f, 0.7f), 1.0f);
			break;
		case TileType::Wall:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color::HSVToRGB({(cosf(elapsedTime * 0.4f) * 0.5f + 0.5f) * 360.0f, 1.0f, 0.9f}), 1.0f);

			modelStack.PushMatrix();

			modelStack.Translate(
				0.0f,
				0.0f,
				0.05f
			);

			RenderMesh(meshList[(int)GeoType::WallTile], true, Color(0.6f, 0.6f, 0.6f), 1.0f);

			modelStack.PopMatrix();
			break;
		case TileType::Soil:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.8f, 0.52f, 0.25f), 1.0f);

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
			break;
		case TileType::Fire:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(1.0f, 0.32f, 0.28f), 1.0f);

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

			RenderMesh(meshList[(int)GeoType::FireTile], true, Color(0.9f, 0.9f, 0.9f), 1.0f);

			modelStack.PopMatrix();
			break;
		case TileType::Water:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.05f, 0.64f, 1.0f), 1.0f);

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
			break;
		case TileType::Grass:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.49f, 0.78f, 0.31f), 1.0f);

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
			break;
		case TileType::Mud:
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.4f, 0.26f, 0.13f), 1.0f);

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
			break;
	}
}

void SceneA2::RenderSceneText(){
	Mesh* const textMesh = meshList[(int)GeoType::Text];
	const float textSize = (float)windowHeight * 0.05f;

	RenderDebugInfoText(textMesh, Color(0.0f, 1.0f, 0.0f), textSize);
	RenderControlsText(textMesh, Color(1.0f, 0.0f, 1.0f), textSize);
	RenderGridAttribsText(textMesh, Color(1.0f, 1.0f, 0.0f), textSize);
	RenderGameInfoText(textMesh, Color(1.0f, 0.5f, 0.0f), textSize);
}

void SceneA2::RenderDebugInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
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

void SceneA2::RenderControlsText(Mesh* const textMesh, const Color& textColor, const float textSize){
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
}

void SceneA2::RenderGridAttribsText(Mesh* const textMesh, const Color& textColor, const float textSize){
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

void SceneA2::RenderGameInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
	const float gameSpd = sim->GetGameSpd();
	RenderTextOnScreen(
		textMesh,
		"Game Spd: " + std::to_string(gameSpd).substr(0, std::to_string((int)gameSpd).length() + 2),
		textColor,
		textSize,
		(float)windowWidth,
		textSize * 19.0f,
		TextAlignment::Right
	);
}