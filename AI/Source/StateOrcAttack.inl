Publisher* StateOrcAttack::im_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateOrcAttack::Enter(Entity* const entity){
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.1f);
	entity->SetTimeLeft(0.0f);
}

void StateOrcAttack::Update(Entity* const entity, const double dt){
	entity->SetSpriteAniElapsedTime(entity->GetSpriteAniElapsedTime() + (float)dt);

	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcExplosive));
		return;
	}

	if(!im_Publisher->Notify((long int)ListenerFlags::ObjPool, new EventFindClosestEnemy(entity), false)){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcPatrol));
		return;
	}

	const Entity* const entityTarget = entity->GetTarget();
	const StateID entityCurrStateID = entityTarget->GetCurrState()->GetID();
	if((entityCurrStateID == StateID::StateSkeleDead)
		|| (entityCurrStateID == StateID::StateReptileDead)
		|| (entityCurrStateID == StateID::StateBoyDead)
		|| (entityCurrStateID == StateID::StateOrcDead)
	){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcChase));
		return;
	}

	const Vector3& entityTargetLocalPos = entityTarget->GetLocalPos();
	const Vector3 vec = Vector3(roundf(entityTargetLocalPos.x), roundf(entityTargetLocalPos.y), roundf(entityTargetLocalPos.z)) - entity->GetLocalPos();
	const float vecLenSquared = vec.LengthSquared();
	if(!(vecLenSquared - 1.0f <= Math::EPSILON && 1.0f - vecLenSquared <= Math::EPSILON)){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcChase));
		return;
	}

	if(entity->GetTimeLeft() <= 0.0f){
		if(entity->GetSpriteAniMiddleName() == "Smack"){
			entity->SetSpriteAniMiddleName("Static");

			entity->SetTimeLeft(0.9f); //Attack cooldown
		} else{
			entity->SetSpriteAniMiddleName("Smack");
			entity->SetSpriteAniElapsedTime(0.0f);

			const long flags = long int(entity->GetTeam() == EntityTeam::Alpha ? ListenerFlags::OmegaTeam : ListenerFlags::AlphaTeam);
			const float dmg = entity->GetDmg();
			const Vector3 dir = entityTargetLocalPos - entity->GetLocalPos();
			const Vector3 perpendicular = RotateVec(dir, Math::DegreeToRadian(90.0f));

			(void)im_Publisher->Notify(flags, new EventAttacking(dmg, entityTargetLocalPos), false);
			(void)im_Publisher->Notify(flags, new EventAttacking(dmg * 0.25f, entityTargetLocalPos + perpendicular), false);
			(void)im_Publisher->Notify(flags, new EventAttacking(dmg * 0.25f, entityTargetLocalPos - perpendicular), false);

			entity->SetTimeLeft(0.7f); //Attack interval
		}
	} else{
		entity->SetTimeLeft(entity->GetTimeLeft() - (float)dt);
	}
	entity->SetGridTargetLocalPos(entity->GetLocalPos() + vec); //No need to normalize as not moving there
}

void StateOrcAttack::Exit(Entity* const entity){
	entity->SetGridTargetLocalPos(entity->GetLocalPos());
}