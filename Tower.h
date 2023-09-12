#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "TouchableObject.h"

class Tower {
public:
	Tower(Model* model);
	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

public:
	Vector3 GetTowerPos()const { return worldTrans.translation_; }
	bool IsDead() { return isDead; };

private:
	Model* model = nullptr;
	WorldTransform worldTrans;
	std::unique_ptr<TouchableObject> touchableObject;

	int hp;
	bool isDead;
};