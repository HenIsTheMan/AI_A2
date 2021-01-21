#pragma once

#include "SceneSupport.h"

#include "Sim.h"
#include "HexGrid.h"
#include "Publisher.h"

class Scene final: public SceneSupport{
public:
	Scene();
	~Scene();

	void Update(double dt) override;
	void Render() override;
private:
	bool shldRenderTileArt;
	bool shldRenderFog;
	float customHue;

	HexGrid<float>::GridType gridType;
	float gridCellScaleX;
	float gridCellScaleY;
	float gridLineThickness;
	int gridRows;
	int gridCols;

	Sim* sim;
	HexGrid<float>* grid;
	Publisher* publisher;

	void UpdateEntities(const double dt);

	void RenderBG();
	void RenderCover();
	void RenderEntities();
	void RenderMap();
	void RenderFog(const std::vector<FogType>& fogLayer, const int r, const int c);
	void RenderTile(const std::vector<TileType>& tileLayer, const int r, const int c);

	void RenderSceneText();
	void RenderDebugInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderControlsText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderGridAttribsText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderSimInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);
};