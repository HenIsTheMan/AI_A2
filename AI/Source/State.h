#pragma once

#include "Vector3.h"
#include "StateID.hpp"

namespace Obj{
	template <class T, typename Type>
	struct Entity;
}

using Entity = Obj::Entity<Vector3, float>;

class State final{ //Shld be templated
public:
	State();
	State(const StateID ID, void (* const enter)(Entity* const entity), void (* const update)(Entity* const entity, const double dt), void (* const exit)(Entity* const entity));
	~State() = default;

	void Enter(Entity* const entity) const;
	void Update(Entity* const entity, const double dt) const;
	void Exit(Entity* const entity) const;

	///Getter
	StateID GetID() const;

	///Setters
	void SetID(const StateID ID);
	void SetEnter(void (* const enter)(Entity* const entity));
	void SetUpdate(void (* const update)(Entity* const entity, const double dt));
	void SetExit(void (* const exit)(Entity* const entity));
private:
	StateID im_ID;

	void (*im_Enter)(Entity* const entity);
	void (*im_Update)(Entity* const entity, const double dt);
	void (*im_Exit)(Entity* const entity);
};