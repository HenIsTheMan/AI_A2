#pragma once

namespace Obj{
	enum struct EntityActionType: int{
		Nil,
		UnidirectionalAttack,
		BidirectionalAttack,
		TridirectionalAttack,
		OmnidirectionalAttack,
		UnidirectionalHeal,
		BidirectionalHeal,
		TridirectionalHeal,
		OmnidirectionalHeal,
		Amt
	};
}