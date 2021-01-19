#pragma once

#include "Event.h"

#include"ListenerFlags.hpp"
#include "Vector3.h"

class EventProcreated final: public Event{
public:
	EventProcreated() = delete;
	EventProcreated(const ListenerFlags& teamFlag, const Vector3& localPos);
	~EventProcreated() = default;

	///Getter
	const ListenerFlags& GetTeamFlag() const;
	const Vector3& GetLocalPos() const;
private:
	ListenerFlags im_TeamFlag;
	Vector3 im_LocalPos;

	EventProcreated* Clone() const override;
};