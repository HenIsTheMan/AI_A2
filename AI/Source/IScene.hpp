#pragma once

class IScene{
public:
	IScene() = default;
	virtual ~IScene() = default;

	virtual void Init() = 0;
	virtual void Update(const double updateDt, const double renderDt) = 0;
	virtual void Render() = 0;
};