#pragma once

#include "Event.h"

class EventGridHeightShrinking final: public Event{
public:
	EventGridHeightShrinking() = delete;
	EventGridHeightShrinking(const int gridRows);
	~EventGridHeightShrinking() = default;

	///Getter
	int GetGridRows() const;
private:
	int im_GridRows;

	EventGridHeightShrinking* Clone() const override;
};