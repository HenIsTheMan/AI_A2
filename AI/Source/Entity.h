#pragma once

#include <cassert>

#include "EntityAttribs.hpp"

#include "Listener.h"

namespace Obj{
	template <class T, typename Type>
	class Entity final: public Listener{
	public:
		Entity<T, Type>();
		Entity<T, Type>(const EntityAttribs<T, Type>& attribs);
		~Entity<T, Type>() = default;

		int OnEvent(Event* myEvent, const bool destroyEvent = false) override;

		//* Getters
		EntityType GetType() const;
		const T& GetLocalPos() const;
		const T& GetLocalScale() const;

		const Type& GetDmg() const;
		const Type& GetCurrHealth() const;
		const Type& GetMaxHealth() const;
		const Type& GetSpd() const;

		const Entity<T, Type>* GetTarget() const;
		const T& GetGridTargetLocalPos() const;
		const Type& GetTimeLeft() const;
		const Type& GetSpriteAniElapsedTime() const;
		const Type& GetSpriteAniDelay() const;
		const EntityTeam GetTeam() const;

		const StateMachine<StateID, Entity<T, Type>>* GetStateMachine() const;
		const State* GetCurrState() const;
		const State* GetNextState() const;
		const std::string& GetSpriteAniMiddleName() const;
		//*/

		//* Setters
		void SetType(const EntityType type);
		void SetLocalPos(const T& localPos);
		void SetLocalScale(const T& localScale);

		void SetDmg(const Type& dmg);
		void SetCurrHealth(const Type& currHealth);
		void SetMaxHealth(const Type& maxHealth);
		void SetSpd(const Type& spd);

		void SetTarget(Entity<T, Type>* const target);
		void SetGridTargetLocalPos(const T& gridTargetLocalPos);
		void SetTimeLeft(const Type& timeLeft);
		void SetSpriteAniElapsedTime(const Type& spriteAniElapsedTime);
		void SetSpriteAniDelay(const Type& spriteAniDelay);
		void SetTeam(const EntityTeam team);

		void SetStateMachine(StateMachine<StateID, Entity<T, Type>>* const stateMachine);
		void SetCurrState(const State* const currState);
		void SetNextState(const State* const nextState);
		void SetSpriteAniMiddleName(const std::string& spriteAniMiddleName);

		#if defined ENTITY_T_IS_VEC3
		void SetLocalPos(const Type& x, const Type& y, const Type& z);
		void SetLocalScale(const Type& x, const Type& y, const Type& z);
		void SetGridTargetLocalPos(const Type& x, const Type& y, const Type& z);
		#endif
		//*/
	private:
		EntityAttribs<T, Type> im_Attribs;
	};
}

#include "Entity.inl"