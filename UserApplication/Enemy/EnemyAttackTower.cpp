#include"EnemyAttackTower.h"
#include"Enemy.h"

void EnemyAttackTower::Initialize() {
	timer = 0;
	cooltime = 30;

}

void EnemyAttackTower::Update(Enemy* enemy, WorldTransform* worldTransform) {
	Attack(enemy, worldTransform);

}

void EnemyAttackTower::Attack(Enemy* enemy, WorldTransform* worldTransform) {
	timer++;
	if (timer >= cooltime) {
		enemy->CreatBullet(worldTransform->translation_, { 0,0,-1 });
		timer = 0;
	}
}