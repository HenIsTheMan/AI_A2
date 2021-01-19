#pragma once

#include "EntityTeam.hpp"
#include "EntityType.hpp"
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
		T im_LocalPos = T();
		T im_LocalScale = T();

		Type im_Dmg = Type();
		Type im_CurrHealth = Type();
		Type im_MaxHealth = Type();
		Type im_Spd = Type();

		Entity<T, Type>* im_Target = nullptr;
		T im_GridTargetLocalPos = T();
		Type im_TimeLeft = Type();
		Type im_SpriteAniElapsedTime = Type();
		Type im_SpriteAniDelay = Type();
		EntityTeam im_Team = EntityTeam::Amt;

		StateMachine<StateID, Entity<T, Type>>* im_StateMachine = nullptr;
		State* im_CurrState = nullptr;
		State* im_NextState = nullptr;
		std::string im_SpriteAniMiddleName = "";
	};
}