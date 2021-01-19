#pragma once

#include "Event.h"

class EventCalcInactiveObjs final: public Event{
public:
	EventCalcInactiveObjs();
	~EventCalcInactiveObjs() = default;
private:
	EventCalcInactiveObjs* Clone() const override;
};