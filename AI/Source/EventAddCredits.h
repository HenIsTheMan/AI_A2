#pragma once

#include "Event.h"

class EventAddCredits final: public Event{
public:
	EventAddCredits() = delete;
	EventAddCredits(const bool toPlayer, const int credits);
	~EventAddCredits() = default;

	//* Getters
	bool GetToPlayer() const;
	int GetCredits() const;
	//*/
private:
	bool im_ToPlayer;
	int im_Credits;

	EventAddCredits* Clone() const override;
};