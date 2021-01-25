#pragma once

namespace Obj{
	enum struct EntityActionType: int{
		Nil,
		UnidirectionalAttack,
		UnidirectionalHeal,
		OmnidirectionalHeal,
		Amt
	};
}