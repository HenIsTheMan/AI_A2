#include "EventFindClosestEnemy.h"

EventFindClosestEnemy::EventFindClosestEnemy(Obj::Entity<Vector3, float>* const entity):
	Event(EventID::EventFindClosestEnemy),
	im_Entity(entity)
{
}

Obj::Entity<Vector3, float>* EventFindClosestEnemy::RetrieveEntity(){
	return im_Entity;
}

EventFindClosestEnemy* EventFindClosestEnemy::Clone() const{
	return new EventFindClosestEnemy(*this);
}