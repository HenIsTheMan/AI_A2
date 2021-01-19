#pragma once
#include "Mesh.h"
#include "Vertex.h"
#include "SpriteAni.h"

class MeshBuilder final{ //Static class
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX = 0.0f, float lengthY = 0.0f, float lengthZ = 0.0f);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenHex(const Color& color, const float radius);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR = 1.f, float innerR = 0.f);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
	static Mesh* GenCircle(Color color, unsigned, float);
	static Mesh* GenerateText(const std::string &meshName, unsigned row, unsigned col);
	static SpriteAni* GenerateSpriteAni(const std::string& meshName, unsigned numRow, unsigned numCol);
};