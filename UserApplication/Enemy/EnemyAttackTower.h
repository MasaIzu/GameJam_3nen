#pragma once
#include"EnemyState.h"

class EnemyAttackTower : public EnemyState {
public:
	void Initialize()override;
	void Update(Enemy* enemy, WorldTransform* worldTransform)override;

private:
	void Attack(Enemy* enemy, WorldTransform* worldTransform);

private:
	int timer;
	int cooltime;
	float bulletSpeed;
};