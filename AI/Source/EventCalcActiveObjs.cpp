#include "EventCalcActiveObjs.h"

EventCalcActiveObjs::EventCalcActiveObjs():
	Event(EventID::EventCalcActiveObjs)
{
}

EventCalcActiveObjs* EventCalcActiveObjs::Clone() const{
	return new EventCalcActiveObjs(*this);
}