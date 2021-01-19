#pragma once

#include <queue>

#include "Event.h"

class Listener{ //Abstract class
public:
	virtual int OnEvent(Event* myEvent, const bool destroyEvent = false) = 0;

	void AddEvent(Event* const myEvent);
	Event* FetchEvent();
protected:
	std::queue<Event*> events;

	Listener();
	virtual ~Listener();
};