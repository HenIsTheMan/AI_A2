#pragma once

#include "Entity.h"
#include "KnightCreationAttribs.h"
#include "GunnerCreationAttribs.h"
#include "HealerCreationAttribs.h"

#include "ObjPool.h"

#include "Singleton.h"

#include "StateMachine.h"

using SM = StateMachine<StateID, Entity>;

namespace Obj{
	template <class T, typename Type>
	class EntityFactory final: public Singleton<EntityFactory<T, Type>>{
		friend Singleton<EntityFactory<T, Type>>;
	public:
		~EntityFactory<T, Type>();

		::Entity* SpawnKnight(const KnightCreationAttribs<T, Type>& attribs);
		::Entity* SpawnGunner(const GunnerCreationAttribs<T, Type>& attribs);
		::Entity* SpawnHealer(const HealerCreationAttribs<T, Type>& attribs);

		::Entity* SpawnRandUnit(const int gridCols, const Sim* const sim, const bool isFlatTop);

		//* Setters
		void SetKnightSM(SM* const knightSM);
		void SetGunnerSM(SM* const gunnerSM);
		void SetHealerSM(SM* const healerSM);
		//*/
	private:
		ObjPool<Entity<T, Type>>* entityPool;

		///Shared SMs
		SM* knightSM;
		SM* gunnerSM;
		SM* healerSM;

		EntityFactory<T, Type>();
	};
}

#include "EntityFactory.inl"