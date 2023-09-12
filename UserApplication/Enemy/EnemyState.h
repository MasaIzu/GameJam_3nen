#pragma once
#include "WorldTransform.h"

class Enemy;

class EnemyState {
public:
	virtual void Initialize() = 0;;
	virtual void Update(Enemy* enemy, WorldTransform* worldTransform) = 0;
};