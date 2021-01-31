Sim* StateAttackKnight::sim = nullptr;
int StateAttackKnight::gridCols = 0;
int* StateAttackKnight::selectedRow = nullptr;
int* StateAttackKnight::selectedCol = nullptr;
int* StateAttackKnight::selectedTargetRow = nullptr;
int* StateAttackKnight::selectedTargetCol = nullptr;

void StateAttackKnight::Enter(Entity* const entity){
	entity->im_Attribs.im_CurrMainStepTime = entity->im_Attribs.im_MaxMainStepTime = 3.0f;
}

void StateAttackKnight::Update(Entity* const entity, const double dt){
	entity->im_Attribs.im_CurrMainStepTime -= (float)dt;

	if(entity->im_Attribs.im_CurrMainStepTime <= 0.0f){
		const Vector3& entityLocalPos = entity->im_Attribs.im_LocalPos;
		*selectedRow = (int)entityLocalPos.y;
		*selectedCol = (int)entityLocalPos.x;

		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleKnight);
	} else if(entity->im_Attribs.im_CurrMainStepTime <= entity->im_Attribs.im_MaxMainStepTime * 0.5f){
		if(*selectedTargetRow >= 0 && *selectedTargetCol >= 0){
			sim->GetEntityLayer()[*selectedTargetRow * gridCols + *selectedTargetCol]->im_Attribs.im_CurrHealth
				-= entity->im_Attribs.im_ActionMag;

			*selectedTargetRow = *selectedTargetCol = -1;
		}
	}
}

void StateAttackKnight::Exit(Entity* const entity){
}