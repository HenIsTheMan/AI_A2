#pragma once

#include <thread>

#include "SceneSupport.h"

#include "Sim.h"
#include "HexGrid.h"
#include "Publisher.h"

class Scene final: public SceneSupport{
public:
	Scene();
	~Scene();

	void Init() override;
	void Update(const double updateDt, const double renderDt) override;
	void Render() override;
private:
	bool canMakeSimMap;
	bool shldRenderTileArt;
	bool shldRenderFog;
	float customHue;
	int creditsPlayer;
	int creditsAI;

	HexGrid<float>::GridType gridType;
	float gridCellScaleX;
	float gridCellScaleY;
	float gridLineThickness;
	int gridRows;
	int gridCols;

	float gridMinCellScaleX;
	float gridMaxCellScaleX;
	float gridMinCellScaleY;
	float gridMaxCellScaleY;
	float gridMinLineThickness;
	float gridMaxLineThickness;
	int gridMinRows;
	int gridMinCols;
	int gridMaxRows;
	int gridMaxCols;

	Sim* sim;
	HexGrid<float>* grid;
	Publisher* publisher;

	std::thread* myThread;

	void UpdateGridAttribs();
	void UpdateMisc(const double dt);
	void UpdateEntities(const double dt);

	void RenderBG();
	void RenderCoverMap();
	void RenderCoverText();
	void RenderEntities();
	void RenderMap();
	void RenderFog(const std::vector<FogType>& fogLayer, const int r, const int c);
	void RenderTile(const std::vector<TileType>& tileLayer, const int r, const int c);

	void RenderSceneText();
	void RenderDebugInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderControlsText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderGridInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderSimInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);

	void MakeSimMap() const;
};