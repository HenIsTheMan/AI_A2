#include "EventBlockPlaced.h"

EventBlockPlaced::EventBlockPlaced(const int blockRow, const int blockCol):
	Event(EventID::EventBlockPlaced),
	im_BlockRow(blockRow),
	im_BlockCol(blockCol)
{
}

int EventBlockPlaced::GetBlockRow() const{
	return im_BlockRow;
}

int EventBlockPlaced::GetBlockCol() const{
	return im_BlockCol;
}

EventBlockPlaced* EventBlockPlaced::Clone() const{
	return new EventBlockPlaced(*this);
}