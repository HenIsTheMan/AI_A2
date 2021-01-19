#include "EventGridHeightShrinking.h"

EventGridHeightShrinking::EventGridHeightShrinking(const int gridRows):
	Event(EventID::EventGridHeightShrinking),
	im_GridRows(gridRows)
{
}

int EventGridHeightShrinking::GetGridRows() const{
	return im_GridRows;
}

EventGridHeightShrinking* EventGridHeightShrinking::Clone() const{
	return new EventGridHeightShrinking(*this);
}