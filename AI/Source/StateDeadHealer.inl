Publisher* StateDeadHealer::sm_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateDeadHealer::Enter(Entity* const entity){
}

void StateDeadHealer::Update(Entity* const entity, const double dt){
	(void)sm_Publisher->Notify((long int)ListenerFlags::Scene, new EventDespawnEntity(entity));
}

void StateDeadHealer::Exit(Entity* const entity){
}