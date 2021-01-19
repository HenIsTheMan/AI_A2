#include "SpriteAni.h"

#include "GL\glew.h"
#include "Vertex.h"
#include <cassert>

SpriteAni::SpriteAni():
	SpriteAni(0, 0)
{
}

SpriteAni::SpriteAni(const int& rows, const int& cols):
	Mesh("notNeeded"),
	rows(rows),
	cols(cols),
	currAni(nullptr),
	allAnis()
{
}

SpriteAni::~SpriteAni(){
	const auto& end = allAnis.end();
	for(auto iter = allAnis.begin(); iter != end; ++iter){
		if(iter->second){
			delete iter->second;
			iter->second = nullptr;
		}
	}
}

void SpriteAni::ActivateAni(const std::string& name, const float delay){
	if(allAnis.find(name) != allAnis.end()){
		currAni = allAnis[name];
		currAni->delay = delay;
	}
}

void SpriteAni::AddAni(const std::string& name, const int& start, const int& end){
	Ani* ani = new Ani();
	for(int i = start; i < end; ++i){
		ani->frames.emplace_back(i);
	}
	allAnis[name] = ani;
}

void SpriteAni::Update(float elapsedTime){
	for(auto& element: allAnis){
		Ani* const& ani = element.second;
		if(ani){
			if(ani->currFrameIndex < 0){
				ani->currFrameIndex = 0;
			} else if(ani->BT <= elapsedTime){
				if(ani->currFrameIndex == ani->frames.size() - 1){
					ani->currFrameIndex = 0;
				} else{
					++ani->currFrameIndex;
				}
				ani->BT = elapsedTime + ani->delay;
			}
		}
	}
}

void SpriteAni::Render() const{
	if(currAni){
		assert(
			currAni->currFrameIndex >= 0
			&& currAni->currFrameIndex <= currAni->frames.back()
			&& "currAni->currFrameIndex is not valid!"
		);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
		if(textureID > 0){
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		if(mode == DRAW_MODE::DRAW_LINES){
			glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(currAni->frames[currAni->currFrameIndex] * 6 * sizeof(GLuint)));
		} else if(mode == DRAW_MODE::DRAW_TRIANGLE_STRIP){
			glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(currAni->frames[currAni->currFrameIndex] * 6 * sizeof(GLuint)));
		} else{
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(currAni->frames[currAni->currFrameIndex] * 6 * sizeof(GLuint)));
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		if(textureID > 0){
			glDisableVertexAttribArray(3);
		}
	}
}

void SpriteAni::ManualRender(const std::string& name, const float time, const float delay) const{
	if(allAnis.find(name) != allAnis.end()){
		Ani* const& myCurrAni = allAnis.at(name);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
		if(textureID > 0){
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		int myAmtOfFrames = (int)myCurrAni->frames.size();
		int myFrameIndex = int(time / delay) % myAmtOfFrames;
		if(mode == DRAW_MODE::DRAW_LINES){
			glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(myCurrAni->frames[myFrameIndex] * 6 * sizeof(GLuint)));
		} else if(mode == DRAW_MODE::DRAW_TRIANGLE_STRIP){
			glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(myCurrAni->frames[myFrameIndex] * 6 * sizeof(GLuint)));
		} else{
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(myCurrAni->frames[myFrameIndex] * 6 * sizeof(GLuint)));
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		if(textureID > 0){
			glDisableVertexAttribArray(3);
		}
	}
}