#pragma once

#include "SceneSupport.h"

#include "HexGrid.h"
#include "ObjPool.h"
#include "Publisher.h"

class SceneA2 final: public SceneSupport{
public:
	SceneA2();
	~SceneA2();

	void Update(double dt) override;
	void Render() override;
private:
	HexGrid<float>::GridType gridType;
	float gridCellWidth;
	float gridCellHeight;
	float gridLineThickness;
	int gridRows;
	int gridCols;

	HexGrid<float>* grid;
	ObjPool<Entity>* objPool;
	Publisher* publisher;

	void RenderGrid();
};