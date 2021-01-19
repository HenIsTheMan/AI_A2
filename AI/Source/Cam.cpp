#include "Cam.h"
#include "App.h"
#include "Mtx44.h"

Cam::Cam(){
	Reset();
}

void Cam::Reset(){
	pos.Set(0.0f, 0.0f, 1.0f);
	target.Set(0.0f, 0.0f, 0.0f);
	up.Set(0.0f, 1.0f, 0.0f);
}

void Cam::Update(double dt){
	const float camSpd = 400.0f;

	if(App::Key('R')){
		return Reset();
	}

	Vector3 temp(0.0f);
	if(App::Key('W')){
		temp.y += 1.0f;
	}
	if(App::Key('S')){
		temp.y -= 1.0f;
	}
	if(App::Key('D')){
		temp.x += 1.0f;
	}
	if(App::Key('A')){
		temp.x -= 1.0f;
	}

	const float dist = temp.Length();
	if(dist > Math::EPSILON || -dist > Math::EPSILON){
		const Vector3 tempNormalized = temp.Normalized();
		pos += tempNormalized * camSpd * (float)dt;
		target += tempNormalized * camSpd * (float)dt;
	}
}