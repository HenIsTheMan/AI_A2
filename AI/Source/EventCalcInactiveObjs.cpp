#include "EventCalcInactiveObjs.h"

EventCalcInactiveObjs::EventCalcInactiveObjs():
	Event(EventID::EventCalcInactiveObjs)
{
}

EventCalcInactiveObjs* EventCalcInactiveObjs::Clone() const{
	return new EventCalcInactiveObjs(*this);
}