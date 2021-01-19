#include "Event.h"

EventID Event::GetID() const{
	return im_ID;
}

Event::Event():
	Event(EventID::Amt)
{
}

Event::Event(const EventID ID):
	im_ID(ID)
{
}