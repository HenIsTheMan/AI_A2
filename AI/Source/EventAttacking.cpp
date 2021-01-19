#include "EventAttacking.h"

EventAttacking::EventAttacking(const float dmg, const Vector3& localPos):
	Event(EventID::EventAttacking),
	im_Dmg(dmg),
	im_LocalPos(localPos)
{
}

float EventAttacking::GetDmg() const{
	return im_Dmg;
}

const Vector3& EventAttacking::GetLocalPos() const{
	return im_LocalPos;
}

EventAttacking* EventAttacking::Clone() const{
	return new EventAttacking(*this);
}