#include"Tower.h"

Tower::Tower(Model* model) {
	this->model = model;
}

void Tower::Initialize() {
	worldTrans.Initialize();
	worldTrans.translation_ = { 0,-30,-540 };
	worldTrans.TransferMatrix();
	touchableObject.reset(TouchableObject::Create(model, worldTrans, COLLISION_ATTR_LANDSHAPE));

	isDead = false;
	hp = 1000;
}

void Tower::Update() {
	if (hp < 0) {
		isDead = true;
	}

	if (touchableObject->GetCollider()->GetHitEnemyAttack()) {

 		hp -= 3;
		touchableObject->GetCollider()->Reset();
	}
}

void Tower::Draw(ViewProjection& viewProjection_) {
	if (isDead == false) {
		model->Draw(worldTrans, viewProjection_);
	}
}