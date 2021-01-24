#include "EntityFactory.h"

namespace Obj{
	EntityFactory::EntityFactory():
		entityPool(Obj::ObjPool<Entity>::RetrieveGlobalObjPtr())
	{
	}

	EntityFactory::~EntityFactory(){
		//try to destroy??
	}

	void EntityFactory::SpawnKnight(const EntityCreationAttribs& attribs){
		Entity* const entity = entityPool->ActivateObj();

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

	void EntityFactory::SpawnGunner(const EntityCreationAttribs& attribs){
	}

	void EntityFactory::SpawnHealer(const EntityCreationAttribs& attribs){
	}

	void EntityFactory::SpawnKing(const EntityCreationAttribs& attribs){
	}

	void EntityFactory::SpawnRandUnit(const EntityCreationAttribs& attribs){
	}
}