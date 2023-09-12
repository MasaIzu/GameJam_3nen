#include"EnemyApproach.h"
#include"Enemy.h"
#include"EnemyAttackTower.h"

void EnemyApproach::Initialize() {
	moveSpeed = 0.3f;
}

void EnemyApproach::Update(Enemy* enemy, WorldTransform* worldTransform) {
	Move(worldTransform);

	if (worldTransform->translation_.z <= 0) {
		enemy->TransitionTo(new EnemyAttackTower);
	}
}

void EnemyApproach::Move(WorldTransform* worldTransform) {
	Vector3 moveVec = towerPos - worldTransform->translation_;
	moveVec.normalize();

	worldTransform->translation_ += moveVec * moveSpeed;
}