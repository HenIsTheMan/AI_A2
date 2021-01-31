#pragma once

#include "EntityTeam.hpp"
#include "EntityType.hpp"
#include "EntityFacingDir.hpp"

#include "EntityActionType.hpp"
#include "EntityVisionType.hpp"

#include "State.h"

enum struct StateID: int;

template <class T, typename Type>
class StateMachine;

namespace Obj{
	template <class T, typename Type>
	struct Entity;

	template <class T, typename Type>
	struct EntityAttribs final{
		EntityAttribs<T, Type>() = default;
		~EntityAttribs<T, Type>() = default;

		EntityType im_Type = EntityType::Null;
		EntityTeam im_Team = EntityTeam::None;
		EntityFacingDir im_FacingDir = EntityFacingDir::Invalid;

		EntityActionType im_ActionType = EntityActionType::Nil;
		int im_ActionRange = 0;
		EntityVisionType im_VisionType = EntityVisionType::Nil;
		int im_VisionRange = 0;

		T im_LocalPos = T();

		Type im_ActionMag = Type();
		Type im_CurrHealth = Type();
		Type im_MaxHealth = Type();
		Type im_TimeAlive = Type();

		//* Vars for specific entities only (help la, I wanna do ECS alr)
		bool im_IdleShldChase = false;
		Type im_IdleChangeFacingDirBT = Type();
		Type im_IdleChangeFacingDirDelay = Type();
		Type im_DeadCurrTime = Type();
		Type im_DeadMaxTime = Type();
		//*/

		T im_GridCellTargetLocalPos = T();
		T im_GridCellStartLocalPos = T();

		StateMachine<StateID, Entity<T, Type>>* im_StateMachine = nullptr;
		State* im_CurrState = nullptr;
		State* im_NextState = nullptr;
	};
}