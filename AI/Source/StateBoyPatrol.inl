#include "EventFindClosestEnemy.h"

float StateBoyPatrol::im_ElapsedTime = 0.0f;
Grid<float>* StateBoyPatrol::im_Grid = nullptr;
Publisher* StateBoyPatrol::im_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateBoyPatrol::Enter(Entity* const entity){
	entity->SetSpriteAniMiddleName("Move");
	entity->SetSpd(1.2f); //Patrol spd
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.1f);
}

void StateBoyPatrol::Update(Entity* const entity, const double dt){
	entity->SetSpriteAniElapsedTime(entity->GetSpriteAniElapsedTime() + (float)dt);

	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyDead));
		return;
	}
	if(im_Publisher->Notify((long int)ListenerFlags::ObjPool, new EventFindClosestEnemy(entity), false)){
		const Entity* const entityTarget = entity->GetTarget();

		if((entityTarget->GetLocalPos() - entity->GetLocalPos()).LengthSquared() <= 4.0f * 4.0f){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyChase));
			return;
		} else{
			entity->SetTarget(nullptr);
		}
	}

	const Vector3 entityGridTargetLocalPos = entity->GetGridTargetLocalPos();
	if((entityGridTargetLocalPos - entity->GetLocalPos()).Length() < entity->GetSpd() * (float)dt){
		entity->SetLocalPos(roundf(entityGridTargetLocalPos.x), roundf(entityGridTargetLocalPos.y), roundf(entityGridTargetLocalPos.z)); //Snap entity's local pos

		if(Math::RandIntMinMax(1, 4) == 1){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyIdle));
			entity->SetTimeLeft(Math::RandFloatMinMax(1.7f, 3.9f));
		} else if(!ChooseADir(entity, im_Grid)){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyIdle));
			entity->SetTimeLeft(0.0f);
		}
	} else{
		MoveInDir(entity, dt);
	}
}

void StateBoyPatrol::Exit(Entity* const entity){
}