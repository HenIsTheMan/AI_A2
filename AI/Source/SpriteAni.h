#pragma once

#include <vector>
#include <unordered_map>

#include "Mesh.h"

class Ani final{
	friend class SpriteAni;
private:
	Ani():
		BT(0.0f),
		delay(0.0f),
		currFrameIndex(-1),
		frames()
	{
	}

	float BT;
	float delay;
	int currFrameIndex;
	std::vector<int> frames;
};

class SpriteAni final: public Mesh{
public:
	SpriteAni();
	SpriteAni(const int& rows, const int& cols);
	~SpriteAni();

	void ActivateAni(const std::string& name, const float BT);
	void AddAni(const std::string& name, const int& start, const int& end);
	void Update(float elapsedTime);
	void Render() const override;
	void ManualRender(const std::string& name, const float time, const float delay) const;
private:
	int rows;
	int cols;

	Ani* currAni;
	std::unordered_map<std::string, Ani*> allAnis;
};