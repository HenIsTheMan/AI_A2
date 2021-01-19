#pragma once

#include "Event.h"

class EventCalcAlphaCount final: public Event{
public:
	EventCalcAlphaCount();
	~EventCalcAlphaCount() = default;
private:
	EventCalcAlphaCount* Clone() const override;
};