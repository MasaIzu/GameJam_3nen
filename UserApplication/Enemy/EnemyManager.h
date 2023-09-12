#pragma once
#include "Enemy.h"
#include <memory>

class EnemyManager {
public:
	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

	void CreateEnemy(Vector3 createPos);
private:
	std::list<std::unique_ptr<Enemy>> enemys;
	

};