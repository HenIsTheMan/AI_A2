#pragma once

#include <thread>

#include "SceneSupport.h"

#include "Entity.h"
#include "EntityFactory.h"
#include "Rounding.hpp"

#include "Sim.h"
#include "HexGrid.h"
#include "ObjPool.h"
#include "Publisher.h"

using Entity = Obj::Entity<Vector3, float>; //Can because Entity<Vector3, float> is in another namespace

class Scene final: public SceneSupport, public Listener{
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
	int spawnCostPlayer;
	int spawnCostAI;

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

	float gridWidth;
	float gridHeight;
	float gridOffsetX;
	float gridOffsetY;
	float mouseRow;
	float mouseCol;

	float gridCellFlatToFlatLen;
	float gridCellSideLen;
	float gridAltOffsetX;
	float gridAltOffsetY;

	Sim* sim;
	HexGrid<float>* grid;
	Obj::EntityFactory<Vector3, float>* entityFactory;
	Obj::ObjPool<Entity>* entityPool;
	Publisher* publisher;

	std::vector<Entity*> entitiesToDeactivate;

	std::thread* myThread;

	void UpdateGridAttribs();
	void UpdateMisc(const double dt);
	void UpdateEntities(const double dt);

	void RenderBG();
	void RenderCoverMap();
	void RenderCoverText();
	void RenderEntities();
	void RenderEntityArt(const Entity* const entity);
	void RenderHealthBar(const Entity* const entity);
	void RenderEntityLvl(const Entity* const entity);
	void RenderMap();
	void RenderTile(const std::vector<TileType>& tileLayer, const int r, const int c);
	void RenderGridCellOfMouse();

	void RenderSceneText();
	void RenderDebugInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderControlsText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderGridInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);
	void RenderSimInfoText(Mesh* const textMesh, const Color& textColor, const float textSize);

	void MakeSimMap();

	int OnEvent(Event* myEvent, const bool destroyEvent = false) override;
};