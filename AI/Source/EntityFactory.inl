namespace Obj{
	template <class T, typename Type>
	EntityFactory<T, Type>::EntityFactory():
		entityPool(ObjPool<Entity<T, Type>>::RetrieveGlobalObjPtr()),
		knightSM(nullptr),
		gunnerSM(nullptr),
		healerSM(nullptr)
	{
	}

	template <class T, typename Type>
	EntityFactory<T, Type>::~EntityFactory(){
		if(entityPool != nullptr){
			entityPool->Destroy();
			entityPool = nullptr;
		}
	}

	template <class T, typename Type>
	::Entity* EntityFactory<T, Type>::SpawnKnight(const KnightCreationAttribs<T, Type>& attribs){
		::Entity* const entity = entityPool->ActivateObj();

		entity->im_Attribs.im_Type = Obj::EntityType::Knight;
		entity->im_Attribs.im_Team = attribs.im_Team;
		entity->im_Attribs.im_FacingDir = attribs.im_FacingDir;
		entity->im_Attribs.im_Lvl = attribs.im_Lvl;

		entity->im_Attribs.im_ActionType = attribs.im_ActionType;
		entity->im_Attribs.im_ActionRange = attribs.im_ActionRange;
		entity->im_Attribs.im_VisionType = attribs.im_VisionType;
		entity->im_Attribs.im_VisionRange = attribs.im_VisionRange;

		entity->im_Attribs.im_LocalPos.x = attribs.im_LocalPos.x;
		entity->im_Attribs.im_LocalPos.y = attribs.im_LocalPos.y;

		entity->im_Attribs.im_ActionMag = attribs.im_ActionMag;
		entity->im_Attribs.im_CurrHealth = attribs.im_Health;
		entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;

		entity->im_Attribs.im_StateMachine = knightSM;
		entity->im_Attribs.im_NextState = knightSM->AcquireState(StateID::StateIdleKnight);

		return entity;
	}

	template <class T, typename Type>
	::Entity* EntityFactory<T, Type>::SpawnGunner(const GunnerCreationAttribs<T, Type>& attribs){
		::Entity* const entity = entityPool->ActivateObj();

		entity->im_Attribs.im_Type = Obj::EntityType::Gunner;
		entity->im_Attribs.im_Team = attribs.im_Team;
		entity->im_Attribs.im_FacingDir = attribs.im_FacingDir;
		entity->im_Attribs.im_Lvl = attribs.im_Lvl;

		entity->im_Attribs.im_ActionType = attribs.im_ActionType;
		entity->im_Attribs.im_ActionRange = attribs.im_ActionRange;
		entity->im_Attribs.im_VisionType = attribs.im_VisionType;
		entity->im_Attribs.im_VisionRange = attribs.im_VisionRange;

		entity->im_Attribs.im_LocalPos.x = attribs.im_LocalPos.x;
		entity->im_Attribs.im_LocalPos.y = attribs.im_LocalPos.y;

		entity->im_Attribs.im_ActionMag = attribs.im_ActionMag;
		entity->im_Attribs.im_CurrHealth = attribs.im_Health;
		entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;

		entity->im_Attribs.im_StateMachine = gunnerSM;
		entity->im_Attribs.im_NextState = gunnerSM->AcquireState(StateID::StateIdleGunner);

		return entity;
	}

	template <class T, typename Type>
	::Entity* EntityFactory<T, Type>::SpawnHealer(const HealerCreationAttribs<T, Type>& attribs){
		::Entity* const entity = entityPool->ActivateObj();

		entity->im_Attribs.im_Type = Obj::EntityType::Healer;
		entity->im_Attribs.im_Team = attribs.im_Team;
		entity->im_Attribs.im_FacingDir = attribs.im_FacingDir;
		entity->im_Attribs.im_Lvl = attribs.im_Lvl;

		entity->im_Attribs.im_ActionType = attribs.im_ActionType;
		entity->im_Attribs.im_ActionRange = attribs.im_ActionRange;
		entity->im_Attribs.im_VisionType = attribs.im_VisionType;
		entity->im_Attribs.im_VisionRange = attribs.im_VisionRange;

		entity->im_Attribs.im_LocalPos.x = attribs.im_LocalPos.x;
		entity->im_Attribs.im_LocalPos.y = attribs.im_LocalPos.y;

		entity->im_Attribs.im_ActionMag = attribs.im_ActionMag;
		entity->im_Attribs.im_CurrHealth = attribs.im_Health;
		entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;

		entity->im_Attribs.im_StateMachine = healerSM;
		entity->im_Attribs.im_NextState = healerSM->AcquireState(StateID::StateIdleHealer);

		return entity;
	}

	template <class T, typename Type>
	::Entity* EntityFactory<T, Type>::SpawnRandUnit(const int gridCols, const Sim* const sim, const bool isFlatTop){
		switch(Math::RandIntMinMax(0, 2)){
			case 0:
				return SpawnKnight(KnightCreationAttribs<Vector3, float>(1, gridCols, sim, isFlatTop));
			case 1:
				return SpawnGunner(GunnerCreationAttribs<Vector3, float>(1, gridCols, sim, isFlatTop));
			case 2:
				return SpawnHealer(HealerCreationAttribs<Vector3, float>(1, gridCols, sim, isFlatTop));
		}
		return nullptr;
	}

	template <class T, typename Type>
	void EntityFactory<T, Type>::SetKnightSM(SM* const knightSM){
		this->knightSM = knightSM;
	}

	template <class T, typename Type>
	void EntityFactory<T, Type>::SetGunnerSM(SM* const gunnerSM){
		this->gunnerSM = gunnerSM;
	}

	template <class T, typename Type>
	void EntityFactory<T, Type>::SetHealerSM(SM* const healerSM){
		this->healerSM = healerSM;
	}
}