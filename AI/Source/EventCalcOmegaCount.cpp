#include "EventCalcOmegaCount.h"

EventCalcOmegaCount::EventCalcOmegaCount():
	Event(EventID::EventCalcOmegaCount)
{
}

EventCalcOmegaCount* EventCalcOmegaCount::Clone() const{
	return new EventCalcOmegaCount(*this);
}