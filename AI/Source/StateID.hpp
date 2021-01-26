#pragma once

enum struct StateID: int{
	StateIdleKnight,
	StatePatrolKnight,
	StateChaseKnight,
	StateAttackKnight,
	StateEscapeKnight,
	StateDeadKnight,

	StateIdleGunner,
	StatePatrolGunner,
	StateChaseGunner,
	StateAttackGunner,
	StateEscapeGunner,
	StateDeadGunner,

	StateWanderHealer,
	StateFollowHealer,
	StateHealHealer,
	StateDeadHealer,

	Amt
};