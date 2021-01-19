#pragma once

#include "Event.h"

#include "Vector3.h"

class EventAttacking final: public Event{
public:
	EventAttacking() = delete;
	EventAttacking(const float dmg, const Vector3& localPos);
	~EventAttacking() = default;

	///Getter
	float GetDmg() const;
	const Vector3& GetLocalPos() const;
private:
	float im_Dmg;
	Vector3 im_LocalPos;

	EventAttacking* Clone() const override;
};