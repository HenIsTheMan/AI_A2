Publisher* StateOrcExplosive::im_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateOrcExplosive::Enter(Entity* const entity){
	entity->SetMaxHealth(80.0f);
	entity->SetCurrHealth(entity->GetMaxHealth());
	entity->SetTimeLeft(7.0f);
	entity->SetTarget(nullptr);
}

void StateOrcExplosive::Update(Entity* const entity, const double dt){
	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcDead));

		const Vector3& entityLocalPos = entity->GetLocalPos();
		const float dmg = entity->GetDmg() * 0.8f;

		(void)im_Publisher->Notify((long int)ListenerFlags::Entity, new EventAttacking(dmg, entityLocalPos + Vector3(1.0f, 0.0f, 0.0f)), false);
		(void)im_Publisher->Notify((long int)ListenerFlags::Entity, new EventAttacking(dmg, entityLocalPos + Vector3(-1.0f, 0.0f, 0.0f)), false);
		(void)im_Publisher->Notify((long int)ListenerFlags::Entity, new EventAttacking(dmg, entityLocalPos + Vector3(0.0f, 1.0f, 0.0f)), false);
		(void)im_Publisher->Notify((long int)ListenerFlags::Entity, new EventAttacking(dmg, entityLocalPos + Vector3(0.0f, -1.0f, 0.0f)), false);

		return;
	}

	if(entity->GetTimeLeft() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateOrcDead));
	} else{
		entity->SetTimeLeft(entity->GetTimeLeft() - (float)dt);
	}
}

void StateOrcExplosive::Exit(Entity* const entity){
	entity->SetCurrHealth(0.0f);
}