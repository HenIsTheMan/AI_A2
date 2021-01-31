#pragma once

enum struct StateID: int{
	StateIdleKnight,
	StatePatrolKnight,
	StateChaseKnight,
	StateAttackKnight,
	StateDeadKnight,

	StateIdleGunner,
	StatePatrolGunner,
	StateChaseGunner,
	StateAttackGunner,
	StateDeadGunner,

	StateIdleHealer,
	StateFollowHealer,
	StateHealHealer,
	StateDeadHealer,

	Amt
};