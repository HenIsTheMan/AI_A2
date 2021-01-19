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
	assert(entity->GetNextState() && "entity->GetNextState() must be initialized!");

	if(entity->GetCurrState() != entity->GetNextState()){
		if(entity->GetCurrState()){
			entity->GetCurrState()->Exit(entity);
			entity->SetCurrState(entity->GetNextState());
			entity->GetCurrState()->Enter(entity);
		} else{
			entity->SetCurrState(entity->GetNextState());
		}
	}
}

template <class T, typename Type>
void StateMachine<T, Type>::UpdateCurrState(Type* const entity, const double dt){
	assert(entity && "entity must be initialized!");
	assert(entity->GetCurrState() && "entity->GetCurrState() must be initialized!");

	entity->GetCurrState()->Update(entity, dt);
}

template <class T, typename Type>
const State* StateMachine<T, Type>::GetState(const T& stateID) const{
	assert(im_States.find(stateID) != im_States.end() && "stateID cannot be found!");

	return im_States.at(stateID);
}