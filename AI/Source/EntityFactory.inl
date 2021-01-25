namespace Obj{
	template<class T, typename Type>
	EntityFactory<T, Type>::EntityFactory():
		entityPool(ObjPool<Entity<T, Type>>::RetrieveGlobalObjPtr())
	{
	}

	template<class T, typename Type>
	EntityFactory<T, Type>::~EntityFactory(){
		if(entityPool != nullptr){
			entityPool->Destroy();
			entityPool = nullptr;
		}
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnKnight(const KnightCreationAttribs<T, Type>& attribs){
		Entity<T, Type>* const entity = entityPool->ActivateObj();

		entity->im_Attribs.im_Type = Obj::EntityType::Knight;
		entity->im_Attribs.im_Team = attribs.im_Team;
		entity->im_Attribs.im_FacingDir = attribs.im_FacingDir;
		entity->im_Attribs.im_Lvl = attribs.im_Lvl;

		entity->im_Attribs.im_LocalPos.x = attribs.im_LocalPos.x;
		entity->im_Attribs.im_LocalPos.y = attribs.im_LocalPos.y;

		entity->im_Attribs.im_CurrHealth = attribs.im_Health;
		entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnGunner(const GunnerCreationAttribs<T, Type>& attribs){
		Entity<T, Type>* const entity = entityPool->ActivateObj();

		entity->im_Attribs.im_Type = Obj::EntityType::Gunner;
		entity->im_Attribs.im_Team = attribs.im_Team;
		entity->im_Attribs.im_FacingDir = attribs.im_FacingDir;
		entity->im_Attribs.im_Lvl = attribs.im_Lvl;

		entity->im_Attribs.im_LocalPos.x = attribs.im_LocalPos.x;
		entity->im_Attribs.im_LocalPos.y = attribs.im_LocalPos.y;

		entity->im_Attribs.im_CurrHealth = attribs.im_Health;
		entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnHealer(const HealerCreationAttribs<T, Type>& attribs){
		Entity<T, Type>* const entity = entityPool->ActivateObj();

		entity->im_Attribs.im_Type = Obj::EntityType::Healer;
		entity->im_Attribs.im_Team = attribs.im_Team;
		entity->im_Attribs.im_FacingDir = attribs.im_FacingDir;
		entity->im_Attribs.im_Lvl = attribs.im_Lvl;

		entity->im_Attribs.im_LocalPos.x = attribs.im_LocalPos.x;
		entity->im_Attribs.im_LocalPos.y = attribs.im_LocalPos.y;

		entity->im_Attribs.im_CurrHealth = attribs.im_Health;
		entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnRandUnit(){
		switch(Math::RandIntMinMax(0, 2)){
			case 0:
				SpawnKnight(KnightCreationAttribs<Vector3, float>());
				break;
			case 1:
				SpawnGunner(GunnerCreationAttribs<Vector3, float>());
				break;
			case 2:
				SpawnHealer(HealerCreationAttribs<Vector3, float>());
				break;
		}
	}
}