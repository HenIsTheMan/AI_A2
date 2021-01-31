Publisher* StateDeadGunner::sm_Publisher = Publisher::RetrieveGlobalObjPtr();
Entity** StateDeadGunner::entityMoving = nullptr;

void StateDeadGunner::Enter(Entity* const entity){
	entity->im_Attribs.im_DeadCurrTime = entity->im_Attribs.im_DeadMaxTime = 4.0f;
}

void StateDeadGunner::Update(Entity* const entity, const double dt){
	if(entity->im_Attribs.im_DeadCurrTime <= 0.0f){
		(void)sm_Publisher->Notify((long int)ListenerFlags::Scene, new EventDespawnEntity(entity));
		*entityMoving = nullptr;
	} else{
		entity->im_Attribs.im_DeadCurrTime -= (float)dt;
	}
}

void StateDeadGunner::Exit(Entity* const entity){
}