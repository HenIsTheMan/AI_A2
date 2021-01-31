bool StateIdleGunner::sm_IsFlatTop = false;

void StateIdleGunner::Enter(Entity* const entity){
	entity->im_Attribs.im_IdleChangeFacingDirDelay = Math::RandFloatMinMax(2.1f, 2.7f);
	entity->im_Attribs.im_IdleChangeFacingDirBT = entity->im_Attribs.im_TimeAlive + entity->im_Attribs.im_IdleChangeFacingDirDelay;
}

void StateIdleGunner::Update(Entity* const entity, const double dt){
	if(entity->im_Attribs.im_CurrHealth <= 0.0f){
		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateDeadGunner);
		return;
	} else if(entity->im_Attribs.im_IdleShldChase){
		entity->im_Attribs.im_NextState = entity->im_Attribs.im_StateMachine->AcquireState(StateID::StateChaseGunner);
	}

	if(entity->im_Attribs.im_IdleChangeFacingDirBT <= entity->im_Attribs.im_TimeAlive){
		if(Math::RandIntMinMax(1, 7) <= 4){
			if(sm_IsFlatTop){
				if(((int)entity->im_Attribs.im_LocalPos.x & 1) == 1){
					switch(entity->im_Attribs.im_FacingDir){
						case Obj::EntityFacingDir::Up:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Left : Obj::EntityFacingDir::Right;
							break;
						case Obj::EntityFacingDir::Down:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::UL : Obj::EntityFacingDir::UR;
							break;
						case Obj::EntityFacingDir::Left:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Up : Obj::EntityFacingDir::Right;
							break;
						case Obj::EntityFacingDir::Right:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Up : Obj::EntityFacingDir::Left;
							break;
						case Obj::EntityFacingDir::UL:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Down : Obj::EntityFacingDir::UR;
							break;
						case Obj::EntityFacingDir::UR:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::UL : Obj::EntityFacingDir::Down;
							break;
					}
				} else{
					switch(entity->im_Attribs.im_FacingDir){
						case Obj::EntityFacingDir::Up:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::DL : Obj::EntityFacingDir::DR;
							break;
						case Obj::EntityFacingDir::Down:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Left : Obj::EntityFacingDir::Right;
							break;
						case Obj::EntityFacingDir::Left:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Right : Obj::EntityFacingDir::Down;
							break;
						case Obj::EntityFacingDir::Right:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Left : Obj::EntityFacingDir::Down;
							break;
						case Obj::EntityFacingDir::DL:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Up : Obj::EntityFacingDir::DR;
							break;
						case Obj::EntityFacingDir::DR:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Up : Obj::EntityFacingDir::DL;
							break;
					}
				}
			} else{
				if(((int)entity->im_Attribs.im_LocalPos.y & 1) == 1){
					switch(entity->im_Attribs.im_FacingDir){
						case Obj::EntityFacingDir::Up:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Right : Obj::EntityFacingDir::Down;
							break;
						case Obj::EntityFacingDir::Down:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Up : Obj::EntityFacingDir::Right;
							break;
						case Obj::EntityFacingDir::Left:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::UR : Obj::EntityFacingDir::DR;
							break;
						case Obj::EntityFacingDir::Right:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Up : Obj::EntityFacingDir::Down;
							break;
						case Obj::EntityFacingDir::DR:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Left : Obj::EntityFacingDir::UR;
							break;
						case Obj::EntityFacingDir::UR:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Left : Obj::EntityFacingDir::DR;
							break;
					}
				} else{
					switch(entity->im_Attribs.im_FacingDir){
						case Obj::EntityFacingDir::Up:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Left : Obj::EntityFacingDir::Down;
							break;
						case Obj::EntityFacingDir::Down:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Up : Obj::EntityFacingDir::Left;
							break;
						case Obj::EntityFacingDir::Left:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::Up : Obj::EntityFacingDir::Down;
							break;
						case Obj::EntityFacingDir::Right:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::UL : Obj::EntityFacingDir::DL;
							break;
						case Obj::EntityFacingDir::DL:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::UL : Obj::EntityFacingDir::Right;
							break;
						case Obj::EntityFacingDir::UL:
							entity->im_Attribs.im_FacingDir = (bool)Math::RandIntMinMax(0, 1) ? Obj::EntityFacingDir::DL : Obj::EntityFacingDir::Right;
							break;
					}
				}
			}
		}

		entity->im_Attribs.im_IdleChangeFacingDirDelay = Math::RandFloatMinMax(2.1f, 2.7f);
		entity->im_Attribs.im_IdleChangeFacingDirBT = entity->im_Attribs.im_TimeAlive + entity->im_Attribs.im_IdleChangeFacingDirDelay;
	}
}

void StateIdleGunner::Exit(Entity* const entity){
	entity->im_Attribs.im_IdleShldChase = false;
}