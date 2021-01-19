#pragma once

#include "Event.h"

#include "Entity.h"

class EventFindClosestEnemy final: public Event{
	EventFindClosestEnemy() = delete;
public:
	EventFindClosestEnemy(Obj::Entity<Vector3, float>* const entity);
	~EventFindClosestEnemy() = default;

	Obj::Entity<Vector3, float>* RetrieveEntity(); 
private:
	Obj::Entity<Vector3, float>* im_Entity;

	EventFindClosestEnemy* Clone() const override;
};