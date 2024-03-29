Publisher* StateDeadKnight::sm_Publisher = Publisher::RetrieveGlobalObjPtr();
Entity** StateDeadKnight::entityMoving = nullptr;

void StateDeadKnight::Enter(Entity* const entity){
	entity->im_Attribs.im_DeadCurrTime = entity->im_Attribs.im_DeadMaxTime = 3.0f;
}

void StateDeadKnight::Update(Entity* const entity, const double dt){
	if(entity->im_Attribs.im_DeadCurrTime <= 0.0f){
		(void)sm_Publisher->Notify((long int)ListenerFlags::Scene, new EventDespawnEntity(entity));
		*entityMoving = nullptr;
	} else{
		entity->im_Attribs.im_DeadCurrTime -= (float)dt;
	}
}

void StateDeadKnight::Exit(Entity* const entity){
}