Grid<float>* StateSkeleChase::im_Grid = nullptr;
Publisher* StateSkeleChase::im_Publisher = Publisher::RetrieveGlobalObjPtr();

void StateSkeleChase::Enter(Entity* const entity){
	entity->SetSpd(1.8f); //Chase spd
	entity->SetSpriteAniElapsedTime(0.0f);
	entity->SetSpriteAniDelay(0.08f);
	entity->SetSpriteAniMiddleName("Move");
}

void StateSkeleChase::Update(Entity* const entity, const double dt){
	entity->SetSpriteAniElapsedTime(entity->GetSpriteAniElapsedTime() + (float)dt);

	if(entity->GetCurrHealth() <= 0.0f){
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateSkeleDeadButNotDead));
		return;
	}

	if(im_Publisher->Notify((long int)ListenerFlags::ObjPool, new EventFindClosestEnemy(entity), false)){
		const Entity* const entityTarget = entity->GetTarget();
		const Vector3& entityTargetLocalPos = entityTarget->GetLocalPos();
		const Vector3& entityLocalPos = entity->GetLocalPos();
		const Vector3& entityGridTargetLocalPos = entity->GetGridTargetLocalPos();

		if((entityTargetLocalPos - entityLocalPos).LengthSquared() > 5.0f * 5.0f){
			entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateSkelePatrol));
			entity->SetTarget(nullptr);
			return;
		}

		if((entityGridTargetLocalPos - entityLocalPos).Length() < entity->GetSpd() * (float)dt){
			entity->SetLocalPos(roundf(entityGridTargetLocalPos.x), roundf(entityGridTargetLocalPos.y), roundf(entityGridTargetLocalPos.z)); //Snap entity's local pos

			const std::vector<std::vector<bool>>& gridBlockData = im_Grid->GetBlockData();
			const std::vector<std::vector<bool>>& gridEntityData = im_Grid->GetEntityData();
			const int gridRows = im_Grid->GetRows();
			const int gridCols = im_Grid->GetCols();

			if(!(((int)entityLocalPos.x + 1 < gridCols
				&& !gridBlockData[(int)entityLocalPos.y][(int)entityLocalPos.x + 1]
				&& !gridEntityData[(int)entityLocalPos.y][(int)entityLocalPos.x + 1])
				|| ((int)entityLocalPos.x - 1 >= 0
				&& !gridBlockData[(int)entityLocalPos.y][(int)entityLocalPos.x - 1]
				&& !gridEntityData[(int)entityLocalPos.y][(int)entityLocalPos.x - 1])
				|| ((int)entityLocalPos.y + 1 < gridRows
				&& !gridBlockData[(int)entityLocalPos.y + 1][(int)entityLocalPos.x]
				&& !gridEntityData[(int)entityLocalPos.y + 1][(int)entityLocalPos.x])
				|| ((int)entityLocalPos.y - 1 >= 0
				&& !gridBlockData[(int)entityLocalPos.y - 1][(int)entityLocalPos.x]
				&& !gridEntityData[(int)entityLocalPos.y - 1][(int)entityLocalPos.x])
			)){
				entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateSkeleIdle));
				return;
			}

			const Vector3 vec = Vector3(roundf(entityTargetLocalPos.x), roundf(entityTargetLocalPos.y), roundf(entityTargetLocalPos.z)) - entityLocalPos;
			if(vec.x <= Math::EPSILON && -vec.x <= Math::EPSILON && vec.y <= Math::EPSILON && -vec.y <= Math::EPSILON){ //If both are very close to 0.0f...
				ChooseADir(entity, im_Grid);
				return;
			}

			const Vector3 dir = Vector3(vec.x / fabs(vec.x), vec.y / fabs(vec.y), vec.z);
			const float vecLenSquared = vec.LengthSquared();
			if(vecLenSquared - 1.0f <= Math::EPSILON && 1.0f - vecLenSquared <= Math::EPSILON){
				entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateSkeleAttack));
				return;
			}

			if(fabs(vec.x) - fabs(vec.y) <= Math::EPSILON && fabs(vec.y) - fabs(vec.x) <= Math::EPSILON){ //If... are equal...
				Vector3 newGridTargetLocalPos[2];
				if(rand() & 1){ //Determines which axis gets checked 1st
					newGridTargetLocalPos[0] = entityLocalPos + Vector3(dir.x, 0.0f, 0.0f);
					newGridTargetLocalPos[1] = entityLocalPos + Vector3(0.0f, dir.y, 0.0f);
				} else{
					newGridTargetLocalPos[0] = entityLocalPos + Vector3(0.0f, dir.y, 0.0f);
					newGridTargetLocalPos[1] = entityLocalPos + Vector3(dir.x, 0.0f, 0.0f);
				}
				const std::vector<std::vector<bool>>& gridBlockData = im_Grid->GetBlockData();
				const std::vector<std::vector<bool>>& gridEntityData = im_Grid->GetEntityData();

				bool move = false;
				for(int i = 0; i < 2; ++i){
					if(!gridBlockData[(int)newGridTargetLocalPos[i].y][(int)newGridTargetLocalPos[i].x]
						&& !gridEntityData[(int)newGridTargetLocalPos[i].y][(int)newGridTargetLocalPos[i].x]
					){ //If grid cell is empty...
						entity->SetGridTargetLocalPos(newGridTargetLocalPos[i]);
						entity->SetSpriteAniElapsedTime(0.0f);
						move = true;
						break;
					}
				}
				if(!move){
					ChooseADir(entity, im_Grid);
				}
			} else{
				Vector3 newGridTargetLocalPos;
				if(fabs(vec.x) > fabs(vec.y)){
					newGridTargetLocalPos = entityLocalPos + Vector3(dir.x, 0.0f, 0.0f);
				} else{
					newGridTargetLocalPos = entityLocalPos + Vector3(0.0f, dir.y, 0.0f);
				}

				const std::vector<std::vector<bool>>& gridBlockData = im_Grid->GetBlockData();
				const std::vector<std::vector<bool>>& gridEntityData = im_Grid->GetEntityData();

				if(!gridBlockData[(int)newGridTargetLocalPos.y][(int)newGridTargetLocalPos.x]
					&& !gridEntityData[(int)newGridTargetLocalPos.y][(int)newGridTargetLocalPos.x]
				){ //If grid cell is empty...
					entity->SetGridTargetLocalPos(newGridTargetLocalPos);
				} else{
					ChooseADir(entity, im_Grid);
				}
			}
		} else{
			MoveInDir(entity, dt);
		}
	} else{
		entity->SetNextState(entity->GetStateMachine()->GetState(StateID::StateSkelePatrol));
		return;
	}
}

void StateSkeleChase::Exit(Entity* const entity){
}