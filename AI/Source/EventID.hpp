#pragma once

enum struct EventID: int{
	EventBlockPlaced,
	EventProcreated,
	EventAttacking,
	EventGridHeightShrinking,
	EventGridWidthShrinking,
	EventCalcActiveObjs,
	EventCalcInactiveObjs,
	EventCalcAlphaCount,
	EventCalcOmegaCount,
	EventFindClosestEnemy,
	Amt
};