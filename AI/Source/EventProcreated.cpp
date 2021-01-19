#include "EventProcreated.h"

EventProcreated::EventProcreated(const ListenerFlags& teamFlag, const Vector3& localPos):
	Event(EventID::EventProcreated),
	im_TeamFlag(teamFlag),
	im_LocalPos(localPos)
{
}

const ListenerFlags& EventProcreated::GetTeamFlag() const{
	return im_TeamFlag;
}

const Vector3& EventProcreated::GetLocalPos() const{
	return im_LocalPos;
}

EventProcreated* EventProcreated::Clone() const{
	return new EventProcreated(*this);
}