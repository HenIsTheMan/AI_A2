bool StateAttackGunner::isDeducted = false;
Sim* StateAttackGunner::sim = nullptr;
int StateAttackGunner::gridCols = 0;
int* StateAttackGunner::selectedRow = nullptr;
int* StateAttackGunner::selectedCol = nullptr;
int* StateAttackGunner::selectedTargetRow = nullptr;
int* StateAttackGunner::selectedTargetCol = nullptr;
int* StateAttackGunner::creditsPlayer = nullptr;
int* StateAttackGunner::creditsAI = nullptr;

void StateAttackGunner::Enter(Entity* const entity){
	entity->im_Attribs.im_CurrActionTime = entity->im_Attribs.im_MaxActionTime = 3.0f;

	//* So will face enemy unit before...
	if((int)entity->im_Attribs.im_GridCellStartLocalPos.x == (int)entity->im_Attribs.im_GridCellTargetLocalPos.x){

		if(entity->im_Attribs.im_GridCellTargetLocalPos.y > entity->im_Attribs.im_GridCellStartLocalPos.y){
			entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Up;
		} else{
			entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Down;
		}

	} else if((int)entity->im_Attribs.im_GridCellStartLocalPos.y == (int)entity->im_Attribs.im_GridCellTargetLocalPos.y){

		if(entity->im_Attribs.im_GridCellTargetLocalPos.x > entity->im_Attribs.im_GridCellStartLocalPos.x){
			entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Right;
		} else{
			entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Left;
		}

	} else{

		if(entity->im_Attribs.im_GridCellTargetLocalPos.y > entity->im_Attribs.im_GridCellStartLocalPos.y){
			if(entity->im_Attribs.im_GridCellTargetLocalPos.x > entity->im_Attribs.im_GridCellStartLocalPos.x){
				entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UR;
			} else{
				entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UL;
			}
		} else{
			if(entity->im_Attribs.im_GridCellTargetLocalPos.x > entity->im_Attribs.im_GridCellStartLocalPos.x){
				entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DR;
			} else{
				entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DL;
			}
		}

	}
	//*/
}

void StateAttackGunner::Update(Entity* const entity, const double dt){
	if(entity->im_Attribs.im_CurrHealth <= 0.0f){
		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateDeadGunner);
		return;
	}
	if(!isDeducted){
		//* Still will minus even if dmg has not been done
		if(sim->turn == SimTurn::Player){
			if(*creditsPlayer >= 10){
				*creditsPlayer -= 10;
			} else{
				entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleGunner);
				return;
			}
		} else if(sim->turn == SimTurn::AI){
			if(*creditsAI >= 10){
				*creditsAI -= 10;
			} else{
				entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleGunner);
				return;
			}
		}

		isDeducted = true;
		//*/
	}

	entity->im_Attribs.im_CurrActionTime -= (float)dt;

	if(entity->im_Attribs.im_CurrActionTime <= 0.0f){
		const Vector3& entityLocalPos = entity->im_Attribs.im_LocalPos;
		*selectedRow = (int)entityLocalPos.y;
		*selectedCol = (int)entityLocalPos.x;

		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleGunner);
	} else if(entity->im_Attribs.im_CurrActionTime <= entity->im_Attribs.im_MaxActionTime * 0.5f){
		if(*selectedTargetRow >= 0 && *selectedTargetCol >= 0){
			Entity* targetEntity = sim->GetEntityLayer()[*selectedTargetRow * gridCols + *selectedTargetCol];

			if(targetEntity != nullptr && targetEntity->im_Attribs.im_Team != entity->im_Attribs.im_Team){
				targetEntity->im_Attribs.im_CurrHealth -= entity->im_Attribs.im_ActionMag;
			}

			*selectedTargetRow = *selectedTargetCol = -1;
		}
	}
}

void StateAttackGunner::Exit(Entity* const entity){
	isDeducted = false;
}