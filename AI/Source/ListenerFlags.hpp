#pragma once

#ifndef BIT
	#define BIT(x) 1 << x
#endif

enum struct ListenerFlags: long int{ //Flags enum
	ObjPool = BIT(1),
	Skele = BIT(2),
	Reptile = BIT(3),
	Boy = BIT(4),
	Orc = BIT(5),
	AlphaTeam = BIT(6),
	OmegaTeam = BIT(7),
	Entity = BIT(8),
	Scene = BIT(9),
	Amt = 9
};