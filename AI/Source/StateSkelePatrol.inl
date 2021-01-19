#include "EventFindClosestEnemy.h"

float StateSkelePatrol::im_ElapsedTime = 0.0f;
Grid<float>* StateSkelePatrol::im_Grid = nullptr;
Publisher* StateSkelePatrol::im_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateSkelePatrol::Enter(Entity* const entity){
	entity->SetSpriteAniMiddleName("Move");
	entity->SetSpd(1.4f); //Patrol spd
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.1f);
}

void StateSkelePatrol::Update(Entity* const entity, const double dt){
	entity->SetSpriteAniElapsedTime(entity->GetSpriteAniElapsedTime() + (float)dt);

	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateSkeleDeadButNotDead));
		return;
	}
	if(im_Publisher->Notify((long int)ListenerFlags::ObjPool, new EventFindClosestEnemy(entity), false)){
		const Entity* const entityTarget = entity->GetTarget();

		if((entityTarget->GetLocalPos() - entity->GetLocalPos()).LengthSquared() <= 5.0f * 5.0f){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateSkeleChase));
			return;
		} else{
			entity->SetTarget(nullptr);
		}
	}

	const Vector3 entityGridTargetLocalPos = entity->GetGridTargetLocalPos();
	if((entityGridTargetLocalPos - entity->GetLocalPos()).Length() < entity->GetSpd() * (float)dt){
		entity->SetLocalPos(roundf(entityGridTargetLocalPos.x), roundf(entityGridTargetLocalPos.y), roundf(entityGridTargetLocalPos.z)); //Snap entity's local pos

		if(!ChooseADir(entity, im_Grid)){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateSkeleIdle));
			entity->SetTimeLeft(0.0f);
		}
	} else{
		MoveInDir(entity, dt);
	}
}

void StateSkelePatrol::Exit(Entity* const entity){
}