#pragma once

#include "Event.h"

class EventCalcActiveObjs final: public Event{
public:
	EventCalcActiveObjs();
	~EventCalcActiveObjs() = default;
private:
	EventCalcActiveObjs* Clone() const override;
};