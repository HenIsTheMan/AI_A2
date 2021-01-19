ObjPool<Entity>* StateReptileDead::im_ObjPool = nullptr;

void StateReptileDead::Enter(Entity* const entity){
	entity->SetTimeLeft(6.0f);
	entity->SetTarget(nullptr);
}

void StateReptileDead::Update(Entity* const entity, const double dt){
	entity->SetTimeLeft(entity->GetTimeLeft() - (float)dt);
	if(entity->GetTimeLeft() <= 0.0f){
		im_ObjPool->DeactivateObj(entity);
	}
}

void StateReptileDead::Exit(Entity* const entity){
}