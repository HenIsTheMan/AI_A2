#pragma once

#include "SceneSupport.h"

#include "TimeOfDay.hpp"

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
	TimeOfDay timeOfDay;

	HexGrid<float>::GridType gridType;
	float gridCellWidth;
	float gridCellHeight;
	float gridLineThickness;
	int gridRows;
	int gridCols;

	HexGrid<float>* grid;
	ObjPool<Entity>* objPool;
	Publisher* publisher;

	void RenderBG();
	void RenderGrid();

	void RenderSceneText();
	void RenderDebugInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderControlsText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderGridAttribsText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderGameInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);
};