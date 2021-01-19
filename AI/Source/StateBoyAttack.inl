Publisher* StateBoyAttack::im_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateBoyAttack::Enter(Entity* const entity){
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.1f);
	entity->SetTimeLeft(0.0f);
}

void StateBoyAttack::Update(Entity* const entity, const double dt){
	entity->SetSpriteAniElapsedTime(entity->GetSpriteAniElapsedTime() + (float)dt);

	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyDead));
		return;
	}

	if(!im_Publisher->Notify((long int)ListenerFlags::ObjPool, new EventFindClosestEnemy(entity), false)){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyPatrol));
		return;
	}

	const Entity* const entityTarget = entity->GetTarget();
	const StateID entityCurrStateID = entityTarget->GetCurrState()->GetID();
	if((entityCurrStateID == StateID::StateSkeleDead)
		|| (entityCurrStateID == StateID::StateReptileDead)
		|| (entityCurrStateID == StateID::StateBoyDead)
		|| (entityCurrStateID == StateID::StateOrcDead)
	){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyChase));
		return;
	}

	const Vector3& entityTargetLocalPos = entityTarget->GetLocalPos();
	const Vector3 vec = Vector3(roundf(entityTargetLocalPos.x), roundf(entityTargetLocalPos.y), roundf(entityTargetLocalPos.z)) - entity->GetLocalPos();
	const float vecLenSquared = vec.LengthSquared();
	if(!(vecLenSquared - 1.0f <= Math::EPSILON && 1.0f - vecLenSquared <= Math::EPSILON)){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyChase));
		return;
	}

	if(entity->GetTimeLeft() <= 0.0f){
		if(Math::RandIntMinMax(1, 10) == 1){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateBoyImmune));
			return;
		}

		if(entity->GetSpriteAniMiddleName() == "Slash"){
			entity->SetSpriteAniMiddleName("Static");

			entity->SetTimeLeft(0.8f); //Attack cooldown
		} else{
			entity->SetSpriteAniMiddleName("Slash");
			entity->SetSpriteAniElapsedTime(0.0f);

			(void)im_Publisher->Notify(long int(entity->GetTeam() == EntityTeam::Alpha ? ListenerFlags::OmegaTeam : ListenerFlags::AlphaTeam),
				new EventAttacking(entity->GetDmg(), entityTargetLocalPos), false);

			entity->SetTimeLeft(0.6f); //Attack interval
		}
	} else{
		entity->SetTimeLeft(entity->GetTimeLeft() - (float)dt);
	}
	entity->SetGridTargetLocalPos(entity->GetLocalPos() + vec); //No need to normalize as not moving there
}

void StateBoyAttack::Exit(Entity* const entity){
	entity->SetGridTargetLocalPos(entity->GetLocalPos());
}