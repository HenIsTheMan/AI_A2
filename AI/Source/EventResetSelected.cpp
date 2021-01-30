#include "EventResetSelected.h"

EventResetSelected::EventResetSelected():
	Event(EventID::EventResetSelected)
{
}

EventResetSelected* EventResetSelected::Clone() const{
	return new EventResetSelected(*this);
}