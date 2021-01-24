#include "SceneSupport.h"
#include "GL\glew.h"

#include "shader.h"
#include "MeshBuilder.h"
#include "App.h"
#include "LoadImg.h"

extern int windowWidth;
extern int windowHeight;

static const int fontWidth[] = { 0,26,26,26,26,26,26,26,26,26,26,26,26,0,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,12,17,21,26,26,37,35,11,16,16,26,26,13,16,13,20,26,26,26,26,26,26,26,26,26,26,14,14,26,26,26,24,46,30,28,28,32,25,24,33,32,13,17,27,22,44,34,34,27,35,28,24,25,33,30,46,27,25,24,16,20,16,26,26,15,25,27,22,27,26,16,24,27,12,12,24,12,42,27,27,27,27,18,20,17,27,23,37,23,24,21,16,24,16,26,26,26,26,13,16,22,36,26,26,21,54,24,18,45,26,24,26,26,13,13,22,22,26,26,47,23,37,20,18,44,26,21,25,12,17,26,26,26,26,26,26,20,43,21,27,26,16,26,20,18,26,17,17,15,29,30,13,16,13,22,27,33,35,35,24,30,30,30,30,30,30,40,28,25,25,25,25,13,13,13,13,32,34,34,34,34,34,34,26,35,33,33,33,33,25,27,27,25,25,25,25,25,25,40,22,26,26,26,26,12,12,12,12,27,27,27,27,27,27,27,26,28,27,27,27,27,24,27,24 };

SceneSupport::SceneSupport():
	im_parameters(),
	meshList(),
	im_vertexArrayID(0),
	im_programID(LoadShaders("Shaders//AI.vertexS", "Shaders//AI.fragS")),
	im_Cam(Cam()),
	modelStack(MS()),
	viewStack(MS()),
	projectionStack(MS()),
	bLightEnabled(false),
	elapsedTime(0.0f),
	updateFPS(0.0f),
	renderFPS(0.0f),
	orthoProjectionScaleFactor(1.0)
{
}

SceneSupport::~SceneSupport(){
	for(int i = 0; i < (int)GeoType::Amt; ++i){
		if(meshList[i]){
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	glDeleteProgram(im_programID);
	glDeleteVertexArrays(1, &im_vertexArrayID);
}

void SceneSupport::Init(){
	Math::InitRNG();

	glClearColor(1.f, 0.82f, 0.86f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &im_vertexArrayID);
	glBindVertexArray(im_vertexArrayID);

	im_parameters[(int)UniType::MVP] = glGetUniformLocation(im_programID, "MVP");
	im_parameters[(int)UniType::Text] = glGetUniformLocation(im_programID, "text");
	im_parameters[(int)UniType::UseTex] = glGetUniformLocation(im_programID, "useTex");
	im_parameters[(int)UniType::UseCustom] = glGetUniformLocation(im_programID, "useCustom");
	im_parameters[(int)UniType::MyAlpha] = glGetUniformLocation(im_programID, "myAlpha");
	im_parameters[(int)UniType::Tex] = glGetUniformLocation(im_programID, "tex");
	im_parameters[(int)UniType::TextColour] = glGetUniformLocation(im_programID, "textColour");
	im_parameters[(int)UniType::MyColour] = glGetUniformLocation(im_programID, "myColour");

	glUseProgram(im_programID);

	for(int i = 0; i < (int)GeoType::Amt; ++i){
		meshList[i] = nullptr;
	}

	meshList[(int)GeoType::Text] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[(int)GeoType::Text]->textureID = LoadImg("Imgs//Calibri.png");
	meshList[(int)GeoType::Text]->material.kAmbient.Set(1, 0, 0);
	meshList[(int)GeoType::TextMod1] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[(int)GeoType::TextMod1]->textureID = LoadImg("Imgs//CalibriMod1.tga");
	meshList[(int)GeoType::TextMod1]->material.kAmbient.Set(1, 0, 0);
	meshList[(int)GeoType::TextMod2] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[(int)GeoType::TextMod2]->textureID = LoadImg("Imgs//CalibriMod2.tga");
	meshList[(int)GeoType::TextMod2]->material.kAmbient.Set(1, 0, 0);

	meshList[(int)GeoType::DayBG] = MeshBuilder::GenerateSpriteAni("DayBG", 3, 5);
	meshList[(int)GeoType::DayBG]->textureID = LoadImg("Imgs//DayBG.png");
	meshList[(int)GeoType::RainyBG] = MeshBuilder::GenerateSpriteAni("RainyBG", 2, 5);
	meshList[(int)GeoType::RainyBG]->textureID = LoadImg("Imgs//RainyBG.png");
	meshList[(int)GeoType::NightBG] = MeshBuilder::GenerateSpriteAni("NightBG", 1, 5);
	meshList[(int)GeoType::NightBG]->textureID = LoadImg("Imgs//NightBG.png");
	SpriteAni* const dayBGSpriteAni = static_cast<SpriteAni*>(meshList[(int)GeoType::DayBG]);
	dayBGSpriteAni->AddAni("DayBG", 0, 12);
	dayBGSpriteAni->ActivateAni("DayBG", 0.07f);
	SpriteAni* const rainyBGSpriteAni = static_cast<SpriteAni*>(meshList[(int)GeoType::RainyBG]);
	rainyBGSpriteAni->AddAni("RainyBG", 0, 10);
	rainyBGSpriteAni->ActivateAni("RainyBG", 0.07f);
	SpriteAni* const nightBGSpriteAni = static_cast<SpriteAni*>(meshList[(int)GeoType::NightBG]);
	nightBGSpriteAni->AddAni("NightBG", 0, 4);
	nightBGSpriteAni->ActivateAni("NightBG", 0.07f);

	meshList[(int)GeoType::Fog] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::Fog]->textureID = LoadImg("Imgs//Fog.png");
	meshList[(int)GeoType::Quad] = MeshBuilder::GenerateQuad("Quad", Color(), 1.0f);
	meshList[(int)GeoType::Circle] = MeshBuilder::GenCircle(Color(), 30, 0.5f);

	meshList[(int)GeoType::Hex] = MeshBuilder::GenHex(Color(), 0.5f);
	meshList[(int)GeoType::WallTile] = MeshBuilder::GenHex(Color(), 0.5f);
	meshList[(int)GeoType::WallTile]->textureID = LoadImg("Imgs//Wall.png");
	meshList[(int)GeoType::SoilTile] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::SoilTile]->textureID = LoadImg("Imgs//Soil.png");
	meshList[(int)GeoType::FireTile] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::FireTile]->textureID = LoadImg("Imgs//Fire.png");
	meshList[(int)GeoType::WaterTile] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::WaterTile]->textureID = LoadImg("Imgs//Water.png");
	meshList[(int)GeoType::GrassTile] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::GrassTile]->textureID = LoadImg("Imgs//Grass.png");
	meshList[(int)GeoType::MudTile] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::MudTile]->textureID = LoadImg("Imgs//Mud.png");

	meshList[(int)GeoType::Knight] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::Knight]->textureID = LoadImg("Imgs//Knight.png");
	meshList[(int)GeoType::Gunner] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::Gunner]->textureID = LoadImg("Imgs//Gunner.png");
	meshList[(int)GeoType::Healer] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::Healer]->textureID = LoadImg("Imgs//Healer.png");
	meshList[(int)GeoType::King] = MeshBuilder::GenerateQuad("", Color(), 1.0f);
	meshList[(int)GeoType::King]->textureID = LoadImg("Imgs//King.png");
}

void SceneSupport::Update(const double updateDt, const double renderDt){
	elapsedTime += (float)updateDt;
	updateFPS = (float)(1.f / updateDt);
	renderFPS = (float)(1.f / renderDt);

	static int polyMode = GL_FILL;
	static bool isF2 = false;

	if(!isF2 && App::Key(VK_F2)){
		polyMode += polyMode == GL_FILL ? -2 : 1;
		glPolygonMode(GL_FRONT_AND_BACK, polyMode);
		isF2 = true;
	} else if(isF2 && !App::Key(VK_F2)){
		isF2 = false;
	}

	static_cast<SpriteAni*>(meshList[(int)GeoType::DayBG])->Update(elapsedTime);
	static_cast<SpriteAni*>(meshList[(int)GeoType::RainyBG])->Update(elapsedTime);
	static_cast<SpriteAni*>(meshList[(int)GeoType::NightBG])->Update(elapsedTime);
}

void SceneSupport::RenderText(Mesh* mesh, std::string text, Color color, TextAlignment alignment){
	if(!mesh || mesh->textureID <= 0){
		return;
	}
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(im_parameters[(int)UniType::UseTex], 1);
	glUniform1i(im_parameters[(int)UniType::Text], 1);
	glUniform3fv(im_parameters[(int)UniType::TextColour], 1, &color.r);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(im_parameters[(int)UniType::Tex], 0);

	const int* myFontWidth = fontWidth;

	float textWidth = 0.0f;
	for(unsigned i = 0; i < text.length(); ++i){
		textWidth += (float)myFontWidth[(unsigned)text[i]] / 64.0f;
	}

	float accum = 0;
	for(unsigned i = 0; i < text.length(); ++i){
		Mtx44 characterSpacing;

		if(alignment == TextAlignment::Right){
			characterSpacing.SetToTranslation(accum + 0.5f - textWidth, 0.5f, 0);
		} else if(alignment == TextAlignment::Center){
			characterSpacing.SetToTranslation(accum + 0.5f - textWidth * 0.5f, 0.5f, 0);
		} else{
			characterSpacing.SetToTranslation(accum + 0.5f, 0.5f, 0);
		}

		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(im_parameters[(int)UniType::MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);

		accum += (float)fontWidth[(unsigned)text[i]] / 64.0f;
	}

	glUniform1i(im_parameters[(int)UniType::Text], 0);
	glUniform1i(im_parameters[(int)UniType::UseTex], 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneSupport::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, TextAlignment alignment){
	if(!mesh || mesh->textureID <= 0){
		return;
	}

	glDisable(GL_DEPTH_TEST);
	glUniform1i(im_parameters[(int)UniType::UseTex], 1);
	glUniform1i(im_parameters[(int)UniType::Text], 1);
	glUniform3fv(im_parameters[(int)UniType::TextColour], 1, &color.r);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(im_parameters[(int)UniType::Tex], 0);

	const int* myFontWidth = fontWidth;

	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0.0f);
	float textWidth = 0.0f;
	for(unsigned i = 0; i < text.length(); ++i){
		textWidth += (float)myFontWidth[(unsigned)text[i]] / 64.0f;
	}
	modelStack.Scale(size, size, 1.0f);

	viewStack.PushMatrix();
	viewStack.LoadIdentity();

	Mtx44 ortho;
	ortho.SetToOrtho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	float accum = 0.0f;
	for(unsigned i = 0; i < text.length(); ++i){
		Mtx44 characterSpacing;

		if(alignment == TextAlignment::Right){
			characterSpacing.SetToTranslation(accum + 0.5f - textWidth, 0.5f, 0);
		} else if(alignment == TextAlignment::Center){
			characterSpacing.SetToTranslation(accum + 0.5f - textWidth * 0.5f, 0.5f, 0);
		} else{
			characterSpacing.SetToTranslation(accum + 0.5f, 0.5f, 0);
		}

		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(im_parameters[(int)UniType::MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);

		accum += (float)fontWidth[(unsigned)text[i]] / 64.0f;
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

	glUniform1i(im_parameters[(int)UniType::Text], 0);
	glUniform1i(im_parameters[(int)UniType::UseTex], 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneSupport::RenderMesh(const Mesh* const& mesh, const bool& useCustom, const Color& colour, const float& alpha){
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	glUniformMatrix4fv(im_parameters[(int)UniType::MVP], 1, GL_FALSE, &MVP.a[0]);
	glUniform1i(im_parameters[(int)UniType::UseCustom], useCustom);
	glUniform3fv(im_parameters[(int)UniType::MyColour], 1, &colour.r);
	glUniform1f(im_parameters[(int)UniType::MyAlpha], alpha);

	if(mesh->textureID){
		glUniform1i(im_parameters[(int)UniType::UseTex], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(im_parameters[(int)UniType::Tex], 0);
	}

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(im_parameters[(int)UniType::UseTex], 0);
	glUniform1i(im_parameters[(int)UniType::UseCustom], 0);
}

void SceneSupport::ManualRenderMesh(const std::string& name, const float time, const float delay, Mesh* const& mesh, const bool& useCustom, const Color& colour, const float& alpha){
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	glUniformMatrix4fv(im_parameters[(int)UniType::MVP], 1, GL_FALSE, &MVP.a[0]);
	glUniform1i(im_parameters[(int)UniType::UseCustom], useCustom);
	glUniform3fv(im_parameters[(int)UniType::MyColour], 1, &colour.r);
	glUniform1f(im_parameters[(int)UniType::MyAlpha], alpha);

	if(mesh->textureID){
		glUniform1i(im_parameters[(int)UniType::UseTex], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(im_parameters[(int)UniType::Tex], 0);
	}

	static_cast<SpriteAni*>(mesh)->ManualRender(name, time, delay);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(im_parameters[(int)UniType::UseTex], 0);
	glUniform1i(im_parameters[(int)UniType::UseCustom], 0);
}

void SceneSupport::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelStack.LoadIdentity();

	viewStack.LoadIdentity();
	viewStack.LookAt(
		im_Cam.pos.x, im_Cam.pos.y, im_Cam.pos.z,
		im_Cam.target.x, im_Cam.target.y, im_Cam.target.z,
		im_Cam.up.x, im_Cam.up.y, im_Cam.up.z
	);

	Mtx44 projection;
	projection.SetToOrtho(
		-(double)windowWidth * 0.5 * orthoProjectionScaleFactor,
		(double)windowWidth * 0.5 * orthoProjectionScaleFactor,
		-(double)windowHeight * 0.5 * orthoProjectionScaleFactor,
		(double)windowHeight * 0.5 * orthoProjectionScaleFactor,
		-99999.0,
		99999.0
	);
	projectionStack.LoadMatrix(projection);
}