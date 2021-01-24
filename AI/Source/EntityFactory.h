#pragma once

#include "Entity.h"
#include "EntityCreationAttribs.hpp"

#include "ObjPool.h"

#include "Singleton.h"

namespace Obj{
	template<class T, typename Type>
	class EntityFactory final: public Singleton<EntityFactory<T, Type>>{
		friend Singleton<EntityFactory<T, Type>>;
	public:
		~EntityFactory<T, Type>();

		void SpawnKnight(const EntityCreationAttribs<T, Type>& attribs);
		void SpawnGunner(const EntityCreationAttribs<T, Type>& attribs);
		void SpawnHealer(const EntityCreationAttribs<T, Type>& attribs);
		void SpawnKing(const EntityCreationAttribs<T, Type>& attribs);

		void SpawnRandUnit(const EntityCreationAttribs<T, Type>& attribs);
	private:
		ObjPool<Entity<T, Type>>* entityPool;

		EntityFactory<T, Type>();
	};
}

#include "EntityFactory.inl"