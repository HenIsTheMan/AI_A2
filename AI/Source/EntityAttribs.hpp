#pragma once

#include "EntityTeam.hpp"
#include "EntityType.hpp"
#include "EntityFacingDir.hpp"

#include "State.h"

enum struct StateID: int;

template <class T, typename Type>
class StateMachine;

namespace Obj{
	template <class T, typename Type>
	class Entity;

	template <class T, typename Type>
	struct EntityAttribs final{
		EntityAttribs<T, Type>() = default;
		~EntityAttribs<T, Type>() = default;

		EntityType im_Type = EntityType::Null;
		EntityTeam im_Team = EntityTeam::None;
		EntityFacingDir im_FacingDir = EntityFacingDir::Invalid;

		T im_LocalPos = T();
		T im_LocalScale = T();

		Type im_Dmg = Type();
		Type im_CurrHealth = Type();
		Type im_MaxHealth = Type();

		Entity<T, Type>* im_EntityTarget = nullptr;
		T im_GridCellTargetLocalPos = T();

		StateMachine<StateID, Entity<T, Type>>* im_StateMachine = nullptr;
		State* im_CurrState = nullptr;
		State* im_NextState = nullptr;
	};
}