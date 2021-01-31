# AI_A2

Features:

- A* pathfinding algorithm
	- Generic
	- Graph-based
	- Supports vertex weights
	- Supports edge weights

## Actions (made up of 1 or more states, can only perform when it's your turn and your unit is idle)

* Do nth (Idle)
* Patrol (Idle -----> Patrol -----> Idle)
* Move to a grid cell (Idle -----> Chase -----> Idle)
* Attack an enemy unit within vision (Idle -----> Chase -----> Attack -----> Idle)

++ what each tile costs and what it does 