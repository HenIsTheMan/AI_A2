namespace Obj{
	template<class T, typename Type>
	EntityFactory<T, Type>::EntityFactory():
		entityPool(ObjPool<Entity<T, Type>>::RetrieveGlobalObjPtr())
	{
	}

	template<class T, typename Type>
	EntityFactory<T, Type>::~EntityFactory(){
		//try to destroy??
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnKnight(const EntityCreationAttribs<T, Type>& attribs){
		Entity<T, Type>* const entity = entityPool->ActivateObj();

		entity->im_Attribs.im_Type = Obj::EntityType::Knight;
		entity->im_Attribs.im_Team = attribs.im_Team;
		entity->im_Attribs.im_FacingDir = attribs.im_FacingDir; //Rotation
		entity->im_Attribs.im_Lvl = attribs.im_Lvl;

		entity->im_Attribs.im_LocalPos.x = attribs.im_LocalPos.x;
		entity->im_Attribs.im_LocalPos.y = attribs.im_LocalPos.y;

		entity->im_Attribs.im_CurrHealth = attribs.im_Health;
		entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;

		//entity->im_Attribs.im_CurrState //Text
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnGunner(const EntityCreationAttribs<T, Type>& attribs){
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnHealer(const EntityCreationAttribs<T, Type>& attribs){
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnKing(const EntityCreationAttribs<T, Type>& attribs){
	}

	template<class T, typename Type>
	void EntityFactory<T, Type>::SpawnRandUnit(const EntityCreationAttribs<T, Type>& attribs){
	}
}