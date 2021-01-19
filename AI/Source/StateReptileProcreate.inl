#include "EventProcreated.h"

Publisher* StateReptileProcreate::im_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateReptileProcreate::Enter(Entity* const entity){
	entity->SetTimeLeft(10.0f);
	entity->SetTarget(nullptr);
}

void StateReptileProcreate::Update(Entity* const entity, const double dt){
	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateReptileDead));
		return;
	}

	if(entity->GetTimeLeft() <= 0.0f){
		(void)im_Publisher->Notify((long int)ListenerFlags::Scene,
			new EventProcreated(entity->GetTeam() == EntityTeam::Alpha ? ListenerFlags::AlphaTeam : ListenerFlags::OmegaTeam, entity->GetLocalPos()), true);
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateReptileAttack));
	} else{
		entity->SetTimeLeft(entity->GetTimeLeft() - (float)dt);
	}
}

void StateReptileProcreate::Exit(Entity* const entity){
	entity->SetTimeLeft(0.0f);
}