#pragma once

#include "Event.h"

class EventResetSelected final: public Event{
public:
	EventResetSelected();
	~EventResetSelected() = default;
private:
	EventResetSelected* Clone() const override;
};