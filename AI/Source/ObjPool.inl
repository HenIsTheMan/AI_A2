#include "EventFindClosestEnemy.h"
#include "StateID.hpp"

enum struct EntityTeam: short;

template <class T>
ObjPool<T>::ObjPool():
	im_ObjPool()
{
}

template <class T>
ObjPool<T>::~ObjPool(){
	const size_t poolSize = im_ObjPool.size();
	for(size_t i = 0; i < poolSize; ++i){
		T*& obj = im_ObjPool[i].second;
		if(obj){
			delete obj;
			obj = nullptr;
		}
	}
}

template <class T>
int ObjPool<T>::OnEvent(Event* myEvent, const bool destroyEvent){
	if(!myEvent){
		return -1;
	}

	int val = -1;

	switch(myEvent->GetID()){
		case EventID::EventCalcActiveObjs: {
			int count = 0;
			const size_t poolSize = im_ObjPool.size();

			for(size_t i = 0; i < poolSize; ++i){
				if(im_ObjPool[i].first){
					++count;
				}
			}

			val = count;
			break;
		}
		case EventID::EventCalcInactiveObjs: {
			int count = 0;
			const size_t poolSize = im_ObjPool.size();

			for(size_t i = 0; i < poolSize; ++i){
				if(!im_ObjPool[i].first){
					++count;
				}
			}

			val = count;
			break;
		}
		case EventID::EventCalcAlphaCount: {
			int count = 0;
			const size_t poolSize = im_ObjPool.size();

			for(size_t i = 0; i < poolSize; ++i){
				if(im_ObjPool[i].first && im_ObjPool[i].second->GetTeam() == EntityTeam::Alpha){
					++count;
				}
			}

			val = count;
			break;
		}
		case EventID::EventCalcOmegaCount: {
			int count = 0;
			const size_t poolSize = im_ObjPool.size();

			for(size_t i = 0; i < poolSize; ++i){
				if(im_ObjPool[i].first && im_ObjPool[i].second->GetTeam() == EntityTeam::Omega){
					++count;
				}
			}

			val = count;
			break;
		}
		case EventID::EventFindClosestEnemy: {
			EventFindClosestEnemy* const eventFindClosestEnemy = static_cast<EventFindClosestEnemy*>(myEvent);
			T* const entity = eventFindClosestEnemy->RetrieveEntity();
			float closestDistSquared = FLT_MAX;
			const size_t poolSize = im_ObjPool.size();

			for(size_t i = 0; i < poolSize; ++i){
				if(im_ObjPool[i].first && im_ObjPool[i].second->GetTeam() != entity->GetTeam()){
					const float currDistSquared = (entity->GetLocalPos() - im_ObjPool[i].second->GetLocalPos()).LengthSquared();
					const StateID currStateID = im_ObjPool[i].second->GetCurrState()->GetID();

					if(currDistSquared < closestDistSquared
						&& (currStateID != StateID::StateSkeleDead)
						&& (currStateID != StateID::StateReptileDead)
						&& (currStateID != StateID::StateBoyDead)
						&& (currStateID != StateID::StateOrcDead)
					){
						closestDistSquared = currDistSquared;
						entity->SetTarget(im_ObjPool[i].second);
					}
				}
			}

			val = (int)entity->GetTarget();
			break;
		}
	}

	if(destroyEvent && myEvent){
		delete myEvent;
		myEvent = nullptr;
	}
	return val;
}

template <class T>
void ObjPool<T>::CreateObjs(int amt){
	assert(im_ObjPool.size() == (size_t)0 && "This func shld only be called once");

	ICreateObjs(amt);
}

template <class T>
void ObjPool<T>::DeactivateObj(const T* const obj){
	const size_t poolSize = im_ObjPool.size();
	for(size_t i = 0; i < poolSize; ++i){
		if(im_ObjPool[i].second == obj){
			im_ObjPool[i].first = false;
			break;
		}
	}
}

template <class T>
T* ObjPool<T>::RetrieveActivatedObj(){
	const size_t poolSize = im_ObjPool.size();
	for(size_t i = 0; i < poolSize; ++i){
		if(!im_ObjPool[i].first){
			im_ObjPool[i].first = true;
			return im_ObjPool[i].second;
		}
	}

	im_ObjPool.push_back({false, new T()});
	return im_ObjPool.back().second;
}

template <class T>
std::vector<std::pair<bool, T*>>& ObjPool<T>::RetrievePool(){
	return im_ObjPool;
}

template <class T>
const std::vector<std::pair<bool, T*>>& ObjPool<T>::GetPool() const{
	return im_ObjPool;
}

template <class T>
void ObjPool<T>::ICreateObjs(const int amt){
	im_ObjPool.resize(amt);
	for(int i = 0; i < amt; ++i){
		im_ObjPool[i] = std::make_pair(false, new T());
	}
}