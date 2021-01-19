#pragma once

#include "Event.h"

class EventCalcOmegaCount final: public Event{
public:
	EventCalcOmegaCount();
	~EventCalcOmegaCount() = default;
private:
	EventCalcOmegaCount* Clone() const override;
};