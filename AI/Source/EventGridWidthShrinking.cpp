#include "EventGridWidthShrinking.h"

EventGridWidthShrinking::EventGridWidthShrinking(const int gridCols):
	Event(EventID::EventGridWidthShrinking),
	im_GridCols(gridCols)
{
}

int EventGridWidthShrinking::GetGridCols() const{
	return im_GridCols;
}

EventGridWidthShrinking* EventGridWidthShrinking::Clone() const{
	return new EventGridWidthShrinking(*this);
}