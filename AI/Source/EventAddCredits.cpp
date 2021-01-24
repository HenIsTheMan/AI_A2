#include "EventAddCredits.h"

EventAddCredits::EventAddCredits(const bool toPlayer, const int credits):
	Event(EventID::EventAddCredits),
	im_ToPlayer(toPlayer),
	im_Credits(credits)
{
}

bool EventAddCredits::GetToPlayer() const{
	return im_ToPlayer;
}

int EventAddCredits::GetCredits() const{
	return im_Credits;
}

EventAddCredits* EventAddCredits::Clone() const{
	return new EventAddCredits(*this);
}