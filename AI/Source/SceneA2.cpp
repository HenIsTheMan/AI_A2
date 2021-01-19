#include "SceneA2.h"

#include "App.h"

extern int windowWidth;
extern int windowHeight;

SceneA2::SceneA2():
	SceneSupport(),
	gameSpd(1.0f),
	timeOfDay(TimeOfDay::Rainy),
	gridType(HexGrid<float>::GridType::FlatTop),
	gridCellWidth(40.0f),
	gridCellHeight(40.0f),
	gridLineThickness(5.0f),
	gridRows(17),
	gridCols(17),
	grid(new HexGrid<float>(HexGrid<float>::GridType::Amt, 0.0f, 0.0f, 0.0f, 0, 0)),
	objPool(new ObjPool<Entity>()),
	publisher(Publisher::RetrieveGlobalObjPtr())
{
	grid->SetGridType(gridType);
	grid->SetCellWidth(gridCellWidth);
	grid->SetCellHeight(gridCellHeight);
	grid->SetLineThickness(gridLineThickness);
	grid->SetRows(gridRows);
	grid->SetCols(gridCols);
}

SceneA2::~SceneA2(){
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
	RenderGrid();
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

	switch(timeOfDay){
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

void SceneA2::RenderGrid(){
	const float gridWidth = grid->CalcWidth();
	const float gridHeight = grid->CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f + gridLineThickness * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f + gridLineThickness * 0.5f;

	const float hexHeight = sin(Math::DegreeToRadian(60)) * gridCellHeight;
	const float hexColOffset = 0.75f * gridCellWidth;
	const float hexOddOffset = hexHeight * 0.5f;

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			modelStack.PushMatrix();
			modelStack.Translate(
				xOffset + (hexColOffset + gridLineThickness) * (float)c,
				yOffset + (hexHeight + gridLineThickness) * (float)r + (c & 1) * hexOddOffset,
				0.05f
			);
			modelStack.Scale(
				gridCellWidth,
				gridCellHeight,
				1.0f
			);
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.5f, 0.5f, 0.5f), 1.0f);
			modelStack.PopMatrix();
		}
	}

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			modelStack.PushMatrix();
			modelStack.Translate(
				xOffset + (hexColOffset + gridLineThickness) * (float)c,
				yOffset + (hexHeight + gridLineThickness) * (float)r + (c & 1) * hexOddOffset,
				0.05f
			);
			modelStack.Scale(
				gridCellWidth + gridLineThickness * 2.0f,
				gridCellHeight + gridLineThickness * 2.0f,
				1.0f
			);
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.0f, 0.0f, 0.0f), 1.0f);
			modelStack.PopMatrix();
		}
	}
}

void SceneA2::RenderSceneText(){
	Mesh* const textMesh = meshList[(int)GeoType::Text];
	const float textSize = (float)windowHeight * 0.05f;
	const float colorComponent = (sinf(elapsedTime * 4.0f) + 1.0f) * 0.5f;

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

void SceneA2::RenderGameInfoText(Mesh* const textMesh, const Color& textColor, const float textSize){
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