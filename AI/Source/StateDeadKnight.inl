Publisher* StateDeadKnight::sm_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateDeadKnight::Enter(Entity* const entity){
}

void StateDeadKnight::Update(Entity* const entity, const double dt){
	(void)sm_Publisher->Notify((long int)ListenerFlags::Scene, new EventDespawnEntity(entity));
}

void StateDeadKnight::Exit(Entity* const entity){
}