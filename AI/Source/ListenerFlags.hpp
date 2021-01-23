#pragma once

#ifndef BIT
	#define BIT(x) 1 << x
#endif

enum struct ListenerFlags: long int{ //Flags enum
	Entity = BIT(0),
	Amt = 1
};