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

		::Entity* SpawnRandUnit();
	private:
		ObjPool<Entity<T, Type>>* entityPool;

		EntityFactory<T, Type>();
	};
}

#include "EntityFactory.inl"