#pragma once
#include <assert.h>
#include <vector>

#include "IScene.hpp"
#include "Cam.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Vertex.h"

enum struct TextAlignment: int{
	Left,
	Center,
	Right,
	Amt
};

class SceneSupport: public IScene{ //Abstract class
public:
	enum struct UniType: int{
		MVP,
		Text,
		UseTex,
		UseCustom,
		MyAlpha,
		Tex,
		TextColour,
		MyColour,
		Amt
	};
	enum struct GeoType: int{
		Text,
		TextMod1,
		TextMod2,
		DayBG,
		RainyBG,
		NightBG,
		Fog,
		Quad,
		Hex,
		WallTile,
		SoilTile,
		FireTile,
		WaterTile,
		GrassTile,
		MudTile,
		Amt
	};

	virtual ~SceneSupport();

	virtual void Init() override;
	virtual void Update(const double updateDt, const double renderDt) override;
	virtual void Render() override;

	void RenderText(Mesh* mesh, std::string text, Color color, TextAlignment alignment = TextAlignment::Left); //Based on what model mats are alr in modelStack
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, TextAlignment alignment = TextAlignment::Left);
	void RenderMesh(const Mesh* const& mesh, const bool& useCustom = false, const Color& colour = Color(), const float& alpha = 1.0f);
	void ManualRenderMesh(const std::string& name, const float time, const float delay,
		Mesh* const& mesh, const bool& useCustom = false, const Color& colour = Color(), const float& alpha = 1.0f);
protected:
	unsigned im_parameters[(int)UniType::Amt];
	Mesh* meshList[(int)GeoType::Amt];
	unsigned im_vertexArrayID;
	unsigned im_programID;

	Cam im_Cam;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	bool bLightEnabled;
	float elapsedTime;
	float updateFPS;
	float renderFPS;

	double orthoProjectionScaleFactor;

	SceneSupport();
};