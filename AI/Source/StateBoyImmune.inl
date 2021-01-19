void StateBoyImmune::Enter(Entity* const entity){
	entity->SetTimeLeft(5.0f);
	entity->SetTarget(nullptr);
}

void StateBoyImmune::Update(Entity* const entity, const double dt){
	if(entity->GetTimeLeft() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyAttack));
	} else{
		entity->SetTimeLeft(entity->GetTimeLeft() - (float)dt);
	}
}

void StateBoyImmune::Exit(Entity* const entity){
	entity->SetTimeLeft(0.0f); //Just in case
}