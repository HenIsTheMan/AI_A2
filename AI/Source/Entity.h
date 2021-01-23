#pragma once

#include <cassert>

#include "EntityAttribs.hpp"

#include "Listener.h"

namespace Obj{
	template <class T, typename Type>
	struct Entity final: public Listener{
	public:
		Entity<T, Type>();
		Entity<T, Type>(const EntityAttribs<T, Type>& attribs);
		~Entity<T, Type>() = default;

		int OnEvent(Event* myEvent, const bool destroyEvent = false) override;
	private:
		EntityAttribs<T, Type> im_Attribs;
	};
}

#include "Entity.inl"