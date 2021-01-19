#pragma once

#include "Event.h"

class EventGridWidthShrinking final: public Event{
public:
	EventGridWidthShrinking() = delete;
	EventGridWidthShrinking(const int gridCols);
	~EventGridWidthShrinking() = default;

	///Getter
	int GetGridCols() const;
private:
	int im_GridCols;

	EventGridWidthShrinking* Clone() const override;
};