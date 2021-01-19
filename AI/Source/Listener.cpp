#include "Listener.h"

void Listener::AddEvent(Event* const myEvent){
	events.emplace(myEvent);
}

Event* Listener::FetchEvent(){
	if(!events.empty()){
		Event* myEvent = events.front();
		events.pop();
		return myEvent;
	} else{
		return nullptr;
	}
}

Listener::Listener():
	events()
{
}

Listener::~Listener(){
	while(!events.empty()){
		Event*& myEvent = events.front();
		if(myEvent){
			delete myEvent;
			myEvent = nullptr;
		}
		events.pop();
	}
};