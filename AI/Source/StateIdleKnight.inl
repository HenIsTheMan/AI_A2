bool StateIdleKnight::sm_IsFlatTop = false;

void StateIdleKnight::Enter(Entity* const entity){
	entity->im_Attribs.im_IdleChangeFacingDirDelay = Math::RandFloatMinMax(2.5f, 3.2f);
	entity->im_Attribs.im_IdleChangeFacingDirBT = entity->im_Attribs.im_TimeAlive + entity->im_Attribs.im_IdleChangeFacingDirDelay;
}

void StateIdleKnight::Update(Entity* const entity, const double dt){
	if(entity->im_Attribs.im_CurrHealth <= 0.0f){
		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateDeadKnight);
	} else if(entity->im_Attribs.im_IdleShldChase){
		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateChaseKnight);
	}

	if(entity->im_Attribs.im_IdleChangeFacingDirBT <= entity->im_Attribs.im_TimeAlive){
		if(Math::RandIntMinMax(1, 10) > 4){
			if(sm_IsFlatTop){
				if(((int)entity->im_Attribs.im_LocalPos.x & 1) == 1){
					switch(Math::RandIntMinMax(1, 6)){
						case 1:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Up;
							break;
						case 2:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Down;
							break;
						case 3:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Left;
							break;
						case 4:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Right;
							break;
						case 5:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UL;
							break;
						case 6:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UR;
							break;
					}
				} else{
					switch(Math::RandIntMinMax(1, 6)){
						case 1:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Up;
							break;
						case 2:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Down;
							break;
						case 3:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Left;
							break;
						case 4:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Right;
							break;
						case 5:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DL;
							break;
						case 6:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DR;
							break;
					}
				}
			} else{
				if(((int)entity->im_Attribs.im_LocalPos.y & 1) == 1){
					switch(Math::RandIntMinMax(1, 6)){
						case 1:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Up;
							break;
						case 2:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Down;
							break;
						case 3:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Left;
							break;
						case 4:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Right;
							break;
						case 5:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DR;
							break;
						case 6:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UR;
							break;
					}
				} else{
					switch(Math::RandIntMinMax(1, 6)){
						case 1:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Up;
							break;
						case 2:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Down;
							break;
						case 3:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Left;
							break;
						case 4:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::Right;
							break;
						case 5:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::DL;
							break;
						case 6:
							entity->im_Attribs.im_FacingDir = Obj::EntityFacingDir::UL;
							break;
					}
				}
			}
		}

		entity->im_Attribs.im_IdleChangeFacingDirDelay = Math::RandFloatMinMax(2.5f, 3.2f);
		entity->im_Attribs.im_IdleChangeFacingDirBT = entity->im_Attribs.im_TimeAlive + entity->im_Attribs.im_IdleChangeFacingDirDelay;
	}
}

void StateIdleKnight::Exit(Entity* const entity){
	entity->im_Attribs.im_IdleShldChase = false;
}