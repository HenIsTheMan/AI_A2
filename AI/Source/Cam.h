#ifndef Cam_H
#define Cam_H

#include "Vector3.h"

class Cam{
public:
	Cam();
	~Cam() = default;
	virtual void Reset();
	virtual void Update(double dt);

	Vector3 pos;
	Vector3 target;
	Vector3 up;
};

#endif