#pragma once

#include "Event.h"

class EventBlockPlaced final: public Event{
	EventBlockPlaced() = delete;
public:
	EventBlockPlaced(const int blockRow, const int blockCol);
	~EventBlockPlaced() = default;

	///Getters
	int GetBlockRow() const;
	int GetBlockCol() const;
private:
	int im_BlockRow;
	int im_BlockCol;

	EventBlockPlaced* Clone() const override;
};