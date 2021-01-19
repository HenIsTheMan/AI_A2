#pragma once

class IScene{
public:
	IScene() = default;
	virtual ~IScene() = default;

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
};