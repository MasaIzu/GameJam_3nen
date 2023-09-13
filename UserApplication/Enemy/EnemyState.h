#pragma once
#include "WorldTransform.h"

class Enemy;

class EnemyState {
public:
	virtual void Initialize() = 0;
	virtual void Update(Enemy* enemy, WorldTransform* worldTransform) = 0;
	static void SetTowerPos(Vector3 TowerPos) { towerPos = TowerPos; };

protected:
	static Vector3 towerPos;
};