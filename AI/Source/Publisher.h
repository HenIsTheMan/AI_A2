#pragma once

#include <vector>

#include "Listener.h"
#include "Singleton.h"

class Publisher final: public Singleton<Publisher>{
	friend Singleton<Publisher>;
public:
	void AddListener(const long int& flags, Listener* const listener);

	int Notify(const long int& flags, Event* myEvent, const bool async = true); //Can send to >= 1
	int Broadcast(Event* myEvent, const bool async = true);
private:
	std::vector<std::pair<long int, Listener*>> im_Listeners;

	Publisher();
	~Publisher() = default;
};