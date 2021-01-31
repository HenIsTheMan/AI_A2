Publisher* StateDeadHealer::sm_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateDeadHealer::Enter(Entity* const entity){
	entity->im_Attribs.im_DeadCurrTime = entity->im_Attribs.im_DeadMaxTime = 4.0f;
}

void StateDeadHealer::Update(Entity* const entity, const double dt){
	if(entity->im_Attribs.im_DeadCurrTime <= 0.0f){
		(void)sm_Publisher->Notify((long int)ListenerFlags::Scene, new EventDespawnEntity(entity));
	} else{
		entity->im_Attribs.im_DeadCurrTime -= (float)dt;
	}
}

void StateDeadHealer::Exit(Entity* const entity){
}