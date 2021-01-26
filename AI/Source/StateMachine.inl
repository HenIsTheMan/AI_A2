template <class T, typename Type>
StateMachine<T, Type>::StateMachine():
	im_States()
{
}

template <class T, typename Type>
StateMachine<T, Type>::~StateMachine<T, Type>(){
	for(std::pair<T, State*> element: im_States){
		delete element.second;
		element.second = nullptr;
	}
}

template <class T, typename Type>
void StateMachine<T, Type>::AddState(State* const state){
	assert(state && "Var 'state' must be initialized!");

	im_States.insert({state->GetID(), state});
}

template <class T, typename Type>
void StateMachine<T, Type>::CheckForStateTransition(Type* const entity){
	assert(entity && "entity must be initialized!");
	assert(entity->im_Attribs.im_NextState && "entity->im_Attribs.im_NextState must be initialized!");

	if(entity->im_Attribs.im_CurrState != entity->im_Attribs.im_NextState){
		if(entity->im_Attribs.im_CurrState){
			entity->im_Attribs.im_CurrState->Exit(entity);
			entity->im_Attribs.im_CurrState = entity->im_Attribs.im_NextState;
			entity->im_Attribs.im_CurrState->Enter(entity);
		} else{
			entity->im_Attribs.im_CurrState = entity->im_Attribs.im_NextState;
		}
	}
}

template <class T, typename Type>
void StateMachine<T, Type>::UpdateCurrState(Type* const entity, const double dt){
	assert(entity && "entity must be initialized!");
	assert(entity->im_Attribs.im_CurrState && "entity->im_Attribs.im_CurrState must be initialized!");

	entity->im_Attribs.im_CurrState->Update(entity, dt);
}

template <class T, typename Type>
State* StateMachine<T, Type>::AcquireState(const T& stateID) const{
	assert(im_States.find(stateID) != im_States.end() && "stateID cannot be found!");

	return im_States.at(stateID);
}

template <class T, typename Type>
const State* StateMachine<T, Type>::GetState(const T& stateID) const{
	assert(im_States.find(stateID) != im_States.end() && "stateID cannot be found!");

	return im_States.at(stateID);
}