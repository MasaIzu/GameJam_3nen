#include"EnemyAttackTower.h"
#include"Enemy.h"

void EnemyAttackTower::Initialize() {
	timer = 0;
	cooltime = 30;
	bulletSpeed = 1.0f;
}

void EnemyAttackTower::Update(Enemy* enemy, WorldTransform* worldTransform) {
	Attack(enemy, worldTransform);

}

void EnemyAttackTower::Attack(Enemy* enemy, WorldTransform* worldTransform) {
	timer++;
	if (timer >= cooltime) {
		Vector3 moveVec = towerPos - worldTransform->translation_;
		moveVec.y += 3.f;
		moveVec.normalize();
		Vector3 velocity = moveVec * bulletSpeed;
		enemy->CreatBullet(worldTransform->translation_, velocity);
		timer = 0;
	}
}