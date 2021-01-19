ObjPool<Entity>* StateBoyDead::im_ObjPool = nullptr;

void StateBoyDead::Enter(Entity* const entity){
	entity->SetTimeLeft(5.0f);
	entity->SetTarget(nullptr);
}

void StateBoyDead::Update(Entity* const entity, const double dt){
	entity->SetTimeLeft(entity->GetTimeLeft() - (float)dt);
	if(entity->GetTimeLeft() <= 0.0f){
		im_ObjPool->DeactivateObj(entity);
	}
}

void StateBoyDead::Exit(Entity* const entity){
}