#include "EventFindClosestEnemy.h"

float StateReptilePatrol::im_ElapsedTime = 0.0f;
Grid<float>* StateReptilePatrol::im_Grid = nullptr;
Publisher* StateReptilePatrol::im_Publisher = Publisher::RetrieveGlobalObjPtr();
Vector3 StateReptilePatrol::im_CommonDirs[2]{Vector3(), Vector3()};

void StateReptilePatrol::Enter(Entity* const entity){
	entity->SetSpriteAniMiddleName("Move");
	entity->SetSpd(1.7f); //Patrol spd
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.1f);
}

void StateReptilePatrol::Update(Entity* const entity, const double dt){
	entity->SetSpriteAniElapsedTime(entity->GetSpriteAniElapsedTime() + (float)dt);

	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateReptileDead));
		return;
	}
	if(im_Publisher->Notify((long int)ListenerFlags::ObjPool, new EventFindClosestEnemy(entity), false)){
		const Entity* const entityTarget = entity->GetTarget();

		if((entityTarget->GetLocalPos() - entity->GetLocalPos()).LengthSquared() <= 6.0f * 6.0f){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateReptileChase));
			return;
		} else{
			entity->SetTarget(nullptr);
		}
	}

	const Vector3 entityGridTargetLocalPos = entity->GetGridTargetLocalPos();
	if((entityGridTargetLocalPos - entity->GetLocalPos()).Length() < entity->GetSpd() * (float)dt){
		entity->SetLocalPos(roundf(entityGridTargetLocalPos.x), roundf(entityGridTargetLocalPos.y), roundf(entityGridTargetLocalPos.z)); //Snap entity's local pos

		if(!ChooseBetween2Dirs(entity, im_Grid, im_CommonDirs)){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateReptileIdle));
			entity->SetTimeLeft(0.0f);
		}
	} else{
		MoveInDir(entity, dt);
	}
}

void StateReptilePatrol::Exit(Entity* const entity){
}