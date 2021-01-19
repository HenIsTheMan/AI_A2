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

	for(int r = 0; r < gridRows; ++r){
		for(int c = 0; c < gridCols; ++c){
			modelStack.PushMatrix();
			modelStack.Translate(
				xOffset + (gridCellWidth + gridLineThickness) * (float)c,
				yOffset + (gridCellHeight + gridLineThickness) * (float)r,
				0.05f
			);
			modelStack.Rotate(90.0f, 1.0f, 0.0f, 0.0f);
			modelStack.Scale(
				gridCellWidth,
				1.0f,
				gridCellHeight
			);
			RenderMesh(meshList[(int)GeoType::Hex], true, Color(0.5f, 0.5f, 0.5f), 1.0f);
			modelStack.PopMatrix();
		}
	}
}