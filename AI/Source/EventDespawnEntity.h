#pragma once

#include "Event.h"

#include "Entity.h"

using Entity = Obj::Entity<Vector3, float>; //Can because Entity<Vector3, float> is in another namespace

class EventDespawnEntity final: public Event{
public:
	EventDespawnEntity() = delete;
	EventDespawnEntity(Entity* const entity);
	~EventDespawnEntity() = default;

	Entity* AcquireEntity() const;
private:
	Entity* im_Entity;

	EventDespawnEntity* Clone() const override;
};