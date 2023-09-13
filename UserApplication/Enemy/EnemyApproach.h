#pragma once
#include"EnemyState.h"

class EnemyApproach : public EnemyState {
public:
	void Initialize()override;
	void Update(Enemy* enemy, WorldTransform* worldTransform)override;

private:
	void Move(WorldTransform* worldTransform);

private:
	float moveSpeed;
};