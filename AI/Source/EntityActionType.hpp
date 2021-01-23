#pragma once

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