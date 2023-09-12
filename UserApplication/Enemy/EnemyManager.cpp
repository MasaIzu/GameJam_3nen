#include"EnemyManager.h"

void EnemyManager::Initialize() {
	CreateEnemy({  10,7,50 });
	CreateEnemy({   0,7,50 });
	CreateEnemy({ -10,7,50 });
}

void EnemyManager::Update() {
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Update();
	}
}

void EnemyManager::Draw(ViewProjection& viewProjection_) {
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Draw(viewProjection_);
	}
}

void EnemyManager::CreateEnemy(Vector3 createPos) {
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(createPos);
	enemys.push_back(std::move(newEnemy));
}