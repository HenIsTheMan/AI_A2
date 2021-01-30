Publisher* StateDeadGunner::sm_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateDeadGunner::Enter(Entity* const entity){
}

void StateDeadGunner::Update(Entity* const entity, const double dt){
	(void)sm_Publisher->Notify((long int)ListenerFlags::Scene, new EventDespawnEntity(entity));
}

void StateDeadGunner::Exit(Entity* const entity){
}