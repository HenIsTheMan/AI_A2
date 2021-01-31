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
	StateChaseHealer,
	StateHealHealer,
	StateDeadHealer,

	Amt
};