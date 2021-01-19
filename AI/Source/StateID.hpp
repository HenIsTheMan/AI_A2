#pragma once

enum struct StateID: int{
	StateSkeleIdle,
	StateSkelePatrol,
	StateSkeleAttack,
	StateSkeleDead,
	StateSkeleChase,
	StateSkeleDeadButNotDead,

	StateReptileIdle,
	StateReptilePatrol,
	StateReptileAttack,
	StateReptileDead,
	StateReptileChase,
	StateReptileProcreate,

	StateBoyIdle,
	StateBoyPatrol,
	StateBoyAttack,
	StateBoyDead,
	StateBoyChase,
	StateBoyImmune,

	StateOrcIdle,
	StateOrcPatrol,
	StateOrcAttack,
	StateOrcDead,
	StateOrcChase,
	StateOrcExplosive,

	Amt
};