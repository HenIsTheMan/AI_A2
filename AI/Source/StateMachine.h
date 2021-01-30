#pragma once

#include <unordered_map>

#include "State.h"

template <class T, typename Type>
class StateMachine final{
public:
	StateMachine<T, Type>();
	~StateMachine<T, Type>();

	void AddState(State* const state);
	void CheckForStateTransition(Type* const entity);
	void UpdateCurrState(Type* const entity, const double dt);

	State* AcquireState(const T& stateID) const;
private:
	std::unordered_map<T, State*> im_States;
};

#include "StateMachine.inl"