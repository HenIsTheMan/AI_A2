#include "EventBlockPlaced.h"
#include "EventAttacking.h"
#include "EventGridHeightShrinking.h"
#include "EventGridWidthShrinking.h"

namespace Obj{
	template <class T, typename Type>
	Entity<T, Type>::Entity():
		Entity(EntityAttribs<T, Type>())
	{
	}

	template <class T, typename Type>
	Entity<T, Type>::Entity(const EntityAttribs<T, Type>& attribs):
		im_Attribs(attribs)
	{
	}

	template <class T, typename Type>
	int Entity<T, Type>::OnEvent(Event* myEvent, const bool destroyEvent){
		if(!myEvent){
			return -1;
		}

		int val = -1;

		switch(myEvent->GetID()){
			case EventID::EventBlockPlaced: {
				const EventBlockPlaced* const eventBlockPlaced = dynamic_cast<const EventBlockPlaced*>(myEvent);
				assert(eventBlockPlaced && "Val of eventBlockPlaced is nullptr!");

				const float blockRow = (float)eventBlockPlaced->GetBlockRow();
				const float blockCol = (float)eventBlockPlaced->GetBlockCol();

				if(im_Attribs.im_LocalPos.x > blockCol - 1.0f && im_Attribs.im_LocalPos.x < blockCol + 1.0f
					&& im_Attribs.im_LocalPos.y > blockRow - 1.0f && im_Attribs.im_LocalPos.y < blockRow + 1.0f){
					val = -5;
				}
				break;
			}
			case EventID::EventAttacking: {
				const EventAttacking* const eventAttacking = dynamic_cast<const EventAttacking*>(myEvent);
				assert(eventAttacking && "Val of eventAttacking is nullptr!");

				const Vector3& eventAttackingLocalPos = eventAttacking->GetLocalPos();
				if(im_Attribs.im_LocalPos.x > eventAttackingLocalPos.x - 1.0f && im_Attribs.im_LocalPos.x < eventAttackingLocalPos.x + 1.0f
					&& im_Attribs.im_LocalPos.y > eventAttackingLocalPos.y - 1.0f && im_Attribs.im_LocalPos.y < eventAttackingLocalPos.y + 1.0f){
					val = 1;
					if(im_Attribs.im_CurrState->GetID() != StateID::StateBoyImmune){
						im_Attribs.im_CurrHealth -= eventAttacking->GetDmg();
					}
				}

				break;
			}
			case EventID::EventGridHeightShrinking: {
				const EventGridHeightShrinking* const eventGridHeightShrinking = dynamic_cast<const EventGridHeightShrinking*>(myEvent);
				assert(eventGridHeightShrinking && "Val of eventGridHeightShrinking is nullptr!");
				const int gridRows = eventGridHeightShrinking->GetGridRows();

				if(im_Attribs.im_LocalPos.y >= (float)gridRows - 1.0f){
					if(!gridRows){
						val = -5;
						break;
					}
					im_Attribs.im_LocalPos.y = float(gridRows - 1);
				}
				if(im_Attribs.im_GridTargetLocalPos.y >= (float)gridRows - 1.0f){
					im_Attribs.im_GridTargetLocalPos.y = float(gridRows - 1);
				}

				break;
			}
			case EventID::EventGridWidthShrinking: {
				const EventGridWidthShrinking* const eventGridWidthShrinking = dynamic_cast<const EventGridWidthShrinking*>(myEvent);
				assert(eventGridWidthShrinking && "Val of eventGridWidthShrinking is nullptr!");
				const int gridCols = eventGridWidthShrinking->GetGridCols();

				if(im_Attribs.im_LocalPos.x >= (float)gridCols - 1.0f){
					if(!gridCols){
						val = -5;
						break;
					}
					im_Attribs.im_LocalPos.x = float(gridCols - 1);
				}
				if(im_Attribs.im_GridTargetLocalPos.x >= (float)gridCols - 1.0f){
					im_Attribs.im_GridTargetLocalPos.x = float(gridCols - 1);
				}
				break;
			}
		}

		if(destroyEvent && myEvent){
			delete myEvent;
			myEvent = nullptr;
		}
		return val;
	}

	template <class T, typename Type>
	EntityType Entity<T, Type>::GetType() const{
		return im_Attribs.im_Type;
	}

	template <class T, typename Type>
	const T& Entity<T, Type>::GetLocalPos() const{
		return im_Attribs.im_LocalPos;
	}

	template <class T, typename Type>
	const T& Entity<T, Type>::GetLocalScale() const{
		return im_Attribs.im_LocalScale;
	}

	template <class T, typename Type>
	const Type& Entity<T, Type>::GetDmg() const{
		return im_Attribs.im_Dmg;
	}

	template <class T, typename Type>
	const Type& Entity<T, Type>::GetCurrHealth() const{
		return im_Attribs.im_CurrHealth;
	}

	template <class T, typename Type>
	const Type& Entity<T, Type>::GetMaxHealth() const{
		return im_Attribs.im_MaxHealth;
	}

	template <class T, typename Type>
	const Type& Entity<T, Type>::GetSpd() const{
		return im_Attribs.im_Spd;
	}

	template <class T, typename Type>
	const Entity<T, Type>* Entity<T, Type>::GetTarget() const{
		return im_Attribs.im_Target;
	}

	template <class T, typename Type>
	const T& Entity<T, Type>::GetGridTargetLocalPos() const{
		return im_Attribs.im_GridTargetLocalPos;
	}

	template <class T, typename Type>
	const Type& Entity<T, Type>::GetTimeLeft() const{
		return im_Attribs.im_TimeLeft;
	}

	template <class T, typename Type>
	const Type& Entity<T, Type>::GetSpriteAniElapsedTime() const{
		return im_Attribs.im_SpriteAniElapsedTime;
	}

	template <class T, typename Type>
	const Type& Entity<T, Type>::GetSpriteAniDelay() const{
		return im_Attribs.im_SpriteAniDelay;
	}

	template <class T, typename Type>
	const EntityTeam Entity<T, Type>::GetTeam() const{
		return im_Attribs.im_Team;
	}

	template <class T, typename Type>
	const StateMachine<StateID, Entity<T, Type>>* Entity<T, Type>::GetStateMachine() const{
		return im_Attribs.im_StateMachine;
	}

	template <class T, typename Type>
	const State* Entity<T, Type>::GetCurrState() const{
		return im_Attribs.im_CurrState;
	}

	template <class T, typename Type>
	const State* Entity<T, Type>::GetNextState() const{
		return im_Attribs.im_NextState;
	}

	template <class T, typename Type>
	const std::string& Entity<T, Type>::GetSpriteAniMiddleName() const{
		return im_Attribs.im_SpriteAniMiddleName;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetType(const EntityType type){
		im_Attribs.im_Type = type;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetLocalPos(const T& localPos){
		im_Attribs.im_LocalPos = localPos;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetLocalScale(const T& localScale){
		im_Attribs.im_LocalScale = localScale;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetDmg(const Type& dmg){
		im_Attribs.im_Dmg = dmg;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetCurrHealth(const Type& currHealth){
		im_Attribs.im_CurrHealth = currHealth;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetMaxHealth(const Type& maxHealth){
		im_Attribs.im_MaxHealth = maxHealth;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetSpd(const Type& spd){
		im_Attribs.im_Spd = spd;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetTarget(Entity<T, Type>* const target){
		im_Attribs.im_Target = target;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetGridTargetLocalPos(const T& gridTargetLocalPos){
		im_Attribs.im_GridTargetLocalPos = gridTargetLocalPos;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetTimeLeft(const Type& timeLeft){
		im_Attribs.im_TimeLeft = timeLeft;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetSpriteAniElapsedTime(const Type& spriteAniElapsedTime){
		im_Attribs.im_SpriteAniElapsedTime = spriteAniElapsedTime;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetSpriteAniDelay(const Type& spriteAniDelay){
		im_Attribs.im_SpriteAniDelay = spriteAniDelay;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetTeam(const EntityTeam team){
		im_Attribs.im_Team = team;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetStateMachine(StateMachine<StateID, Entity<T, Type>>* const stateMachine){
		im_Attribs.im_StateMachine = stateMachine;
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetCurrState(const State* const currState){
		im_Attribs.im_CurrState = const_cast<State* const>(currState);
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetNextState(const State* const nextState){
		im_Attribs.im_NextState = const_cast<State* const>(nextState);
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetSpriteAniMiddleName(const std::string& spriteAniMiddleName){
		im_Attribs.im_SpriteAniMiddleName = spriteAniMiddleName;
	}

	#if defined ENTITY_T_IS_VEC3
	template <class T, typename Type>
	void Entity<T, Type>::SetLocalPos(const Type& x, const Type& y, const Type& z){
		im_Attribs.im_LocalPos = T(x, y, z);
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetLocalScale(const Type& x, const Type& y, const Type& z){
		im_Attribs.im_LocalScale = T(x, y, z);
	}

	template <class T, typename Type>
	void Entity<T, Type>::SetGridTargetLocalPos(const Type& x, const Type& y, const Type& z){
		im_Attribs.im_GridTargetLocalPos = T(x, y, z);
	}
	#endif
}