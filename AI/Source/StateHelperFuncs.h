#pragma once

#include <vector>

#include "Entity.h"
#include "Grid.h"
#include "ObjPool.h"
#include "Publisher.h"

using Entity = Obj::Entity<Vector3, float>;

Vector3 RotateVec(const Vector3& vec, const float& angleInRad);

bool ChooseADir(Entity* const entity, const Grid<float>* const grid);
void ChooseRandDir(Entity* const entity);
bool ChooseBetween2Dirs(Entity* const entity, const Grid<float>* const grid, const Vector3 (&commonDirs)[2]);
void ChooseRandPairOfPerpendicularDirs(Vector3 (&commonDirs)[2]);

void MoveInDir(Entity* const entity, const double dt);