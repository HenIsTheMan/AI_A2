#include "State.h"

State::State():
	State(StateID::Amt, nullptr, nullptr, nullptr)
{
}

State::State(const StateID ID, void (* const enter)(Entity* const entity), void (* const update)(Entity* const entity, const double dt), void (* const exit)(Entity* const entity)):
	im_ID(ID),
	im_Enter(enter),
	im_Update(update),
	im_Exit(exit)
{
}

StateID State::GetID() const{
	return im_ID;
}

void State::SetID(const StateID ID){
	im_ID = ID;
}

void State::SetEnter(void (* const enter)(Entity* const entity)){
	im_Enter = enter;
}

void State::SetUpdate(void (* const update)(Entity* const entity, const double dt)){
	im_Update = update;
}

void State::SetExit(void (* const exit)(Entity* const entity)){
	im_Exit = exit;
}

void State::Enter(Entity* const entity) const{
	if(im_Enter){
		im_Enter(entity);
	}
}

void State::Update(Entity* const entity, const double dt) const{
	if(im_Update){
		im_Update(entity, dt);
	}
}

void State::Exit(Entity* const entity) const{
	if(im_Exit){
		im_Exit(entity);
	}
}