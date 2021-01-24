#pragma once

#include "Entity.h"
#include "EntityCreationAttribs.hpp"

#include "ObjPool.h"

#include "Singleton.h"

using Entity = Obj::Entity<Vector3, float>; //Can because Entity<Vector3, float> is in another namespace

namespace Obj{
	class EntityFactory final: public Singleton<EntityFactory>{
		friend Singleton<EntityFactory>;
	public:
		~EntityFactory();

		void SpawnKnight(const EntityCreationAttribs& attribs);
		void SpawnGunner(const EntityCreationAttribs& attribs);
		void SpawnHealer(const EntityCreationAttribs& attribs);
		void SpawnKing(const EntityCreationAttribs& attribs);

		void SpawnRandUnit(const EntityCreationAttribs& attribs);
	private:
		Obj::ObjPool<::Entity>* entityPool;

		EntityFactory();
	};
}