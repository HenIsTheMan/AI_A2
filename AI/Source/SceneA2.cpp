#include "SceneA2.h"

extern int windowWidth;
extern int windowHeight;

SceneA2::SceneA2():
	gridType(HexGrid<float>::GridType::FlatTop),
	gridCellWidth(40.0f),
	gridCellHeight(40.0f),
	gridLineThickness(2.5f),
	gridRows(20),
	gridCols(20),
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
}

void SceneA2::Render(){
	SceneSupport::Render();

	modelStack.PushMatrix();
	modelStack.Translate(
		-(float)windowWidth * 0.5f,
		-(float)windowHeight * 0.5f,
		0.0f
	);
	RenderGrid();
	modelStack.PopMatrix();
}

void SceneA2::RenderGrid(){
	const float gridWidth = grid->CalcWidth();
	const float gridHeight = grid->CalcHeight();

	const float xOffset = ((float)windowWidth - gridWidth) * 0.5f + gridLineThickness * 0.5f;
	const float yOffset = ((float)windowHeight - gridHeight) * 0.5f + gridLineThickness * 0.5f;

	const int amtOfVertLines = grid->CalcAmtOfVertLines();
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

	const int amtOfHorizLines = grid->CalcAmtOfHorizLines();
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