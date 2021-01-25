#pragma once

#include "Entity.h"
#include "KnightCreationAttribs.h"
#include "GunnerCreationAttribs.h"
#include "HealerCreationAttribs.h"

#include "ObjPool.h"

#include "Singleton.h"

namespace Obj{
	template <class T, typename Type>
	class EntityFactory final: public Singleton<EntityFactory<T, Type>>{
		friend Singleton<EntityFactory<T, Type>>;
	public:
		~EntityFactory<T, Type>();

		::Entity* SpawnKnight(const KnightCreationAttribs<T, Type>& attribs);
		::Entity* SpawnGunner(const GunnerCreationAttribs<T, Type>& attribs);
		::Entity* SpawnHealer(const HealerCreationAttribs<T, Type>& attribs);

		::Entity* SpawnRandUnit(const int gridCols, const Sim* const sim);

		::Entity* SpawnTemp(const KnightCreationAttribs<T, Type>& attribs, const Vector3& localPos, const EntityFacingDir facingDir){
			::Entity* const entity = entityPool->ActivateObj();

			entity->im_Attribs.im_Type = Obj::EntityType::Knight;
			entity->im_Attribs.im_Team = attribs.im_Team;
			entity->im_Attribs.im_FacingDir = facingDir;
			entity->im_Attribs.im_Lvl = attribs.im_Lvl;

			entity->im_Attribs.im_ActionType = attribs.im_ActionType;
			entity->im_Attribs.im_ActionRange = attribs.im_ActionRange;
			entity->im_Attribs.im_VisionType = attribs.im_VisionType;
			entity->im_Attribs.im_VisionRange = attribs.im_VisionRange;

			entity->im_Attribs.im_LocalPos.x = localPos.x;
			entity->im_Attribs.im_LocalPos.y = localPos.y;

			entity->im_Attribs.im_ActionMag = attribs.im_ActionMag;
			entity->im_Attribs.im_CurrHealth = attribs.im_Health;
			entity->im_Attribs.im_MaxHealth = entity->im_Attribs.im_CurrHealth;

			return entity;
		}
	private:
		ObjPool<Entity<T, Type>>* entityPool;

		EntityFactory<T, Type>();
	};
}

#include "EntityFactory.inl"