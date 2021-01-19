#pragma once

#include "EventID.hpp"

///Better if templated

class Event{ //Abstract class
public:
	virtual ~Event() = default;

	virtual Event* Clone() const = 0;

	///Getter
	EventID GetID() const;
protected:
	EventID im_ID;

	Event();
	Event(const EventID ID);
};