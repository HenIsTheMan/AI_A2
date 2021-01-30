#include "EventDespawnEntity.h"

EventDespawnEntity::EventDespawnEntity(Entity* const entity):
	Event(EventID::EventDespawnEntity),
	im_Entity(entity)
{
}

Entity* EventDespawnEntity::AcquireEntity() const{
	return im_Entity;
}

EventDespawnEntity* EventDespawnEntity::Clone() const{
	return new EventDespawnEntity(*this);
}