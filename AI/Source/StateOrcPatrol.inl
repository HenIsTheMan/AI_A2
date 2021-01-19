#include "EventFindClosestEnemy.h"

float StateOrcPatrol::im_ElapsedTime = 0.0f;
Grid<float>* StateOrcPatrol::im_Grid = nullptr;
Publisher* StateOrcPatrol::im_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateOrcPatrol::Enter(Entity* const entity){
	entity->SetSpriteAniMiddleName("Move");
	entity->SetSpd(0.8f); //Patrol spd
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.1f);
}

void StateOrcPatrol::Update(Entity* const entity, const double dt){
	entity->SetSpriteAniElapsedTime(entity->GetSpriteAniElapsedTime() + (float)dt);

	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcExplosive));
		return;
	}
	if(im_Publisher->Notify((long int)ListenerFlags::ObjPool, new EventFindClosestEnemy(entity), false)){
		const Entity* const entityTarget = entity->GetTarget();

		if((entityTarget->GetLocalPos() - entity->GetLocalPos()).LengthSquared() <= 3.0f * 3.0f){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcChase));
			return;
		} else{
			entity->SetTarget(nullptr);
		}
	}

	const Vector3 entityGridTargetLocalPos = entity->GetGridTargetLocalPos();
	if((entityGridTargetLocalPos - entity->GetLocalPos()).Length() < entity->GetSpd() * (float)dt){
		entity->SetLocalPos(roundf(entityGridTargetLocalPos.x), roundf(entityGridTargetLocalPos.y), roundf(entityGridTargetLocalPos.z)); //Snap entity's local pos

		if(!ChooseADir(entity, im_Grid)){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcIdle));
			entity->SetTimeLeft(0.0f);
			return;
		}

		if(entity->GetTimeLeft() > 999.0f){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcIdle));
			entity->SetTimeLeft(Math::RandFloatMinMax(1.5f, 4.0f));
		} else{
			entity->SetTimeLeft(1000.0f);
		}
	} else{
		MoveInDir(entity, dt);
	}
}

void StateOrcPatrol::Exit(Entity* const entity){
}