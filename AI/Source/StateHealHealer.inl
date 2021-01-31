bool StateHealHealer::isDeducted = false;
Sim* StateHealHealer::sim = nullptr;
int StateHealHealer::gridCols = 0;
int* StateHealHealer::selectedRow = nullptr;
int* StateHealHealer::selectedCol = nullptr;
int* StateHealHealer::selectedTargetRow = nullptr;
int* StateHealHealer::selectedTargetCol = nullptr;
int* StateHealHealer::creditsPlayer = nullptr;
int* StateHealHealer::creditsAI = nullptr;

void StateHealHealer::Enter(Entity* const entity){
	entity->im_Attribs.im_CurrActionTime = entity->im_Attribs.im_MaxActionTime = 4.0f;

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

void StateHealHealer::Update(Entity* const entity, const double dt){
	if(entity->im_Attribs.im_CurrHealth <= 0.0f){
		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateDeadHealer);
		return;
	}
	if(!isDeducted){
		//* Still will minus even if dmg has not been done
		if(sim->turn == SimTurn::Player){
			if(*creditsPlayer >= 7){
				*creditsPlayer -= 7;
			} else{
				entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleHealer);
				return;
			}
		} else if(sim->turn == SimTurn::AI){
			if(*creditsAI >= 7){
				*creditsAI -= 7;
			} else{
				entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleHealer);
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

		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateIdleHealer);
	} else if(entity->im_Attribs.im_CurrActionTime <= entity->im_Attribs.im_MaxActionTime * 0.5f){
		if(*selectedTargetRow >= 0 && *selectedTargetCol >= 0){
			Entity* targetEntity = sim->GetEntityLayer()[*selectedTargetRow * gridCols + *selectedTargetCol];

			if(targetEntity != nullptr && targetEntity->im_Attribs.im_Team == entity->im_Attribs.im_Team){
				targetEntity->im_Attribs.im_CurrHealth += entity->im_Attribs.im_ActionMag;
			}

			*selectedTargetRow = *selectedTargetCol = -1;
		}
	}
}

void StateHealHealer::Exit(Entity* const entity){
	isDeducted = false;
}