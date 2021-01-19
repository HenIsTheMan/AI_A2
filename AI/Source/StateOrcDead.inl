ObjPool<Entity>* StateOrcDead::im_ObjPool = nullptr;

void StateOrcDead::Enter(Entity* const entity){
	entity->SetTimeLeft(7.0f);
	entity->SetTarget(nullptr);
}

void StateOrcDead::Update(Entity* const entity, const double dt){
	entity->SetTimeLeft(entity->GetTimeLeft() - (float)dt);
	if(entity->GetTimeLeft() <= 0.0f){
		im_ObjPool->DeactivateObj(entity);
	}
}

void StateOrcDead::Exit(Entity* const entity){
}