#include "EventCalcAlphaCount.h"

EventCalcAlphaCount::EventCalcAlphaCount():
	Event(EventID::EventCalcAlphaCount)
{
}

EventCalcAlphaCount* EventCalcAlphaCount::Clone() const{
	return new EventCalcAlphaCount(*this);
}