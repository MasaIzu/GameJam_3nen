#include"Tower.h"

Tower::Tower(Model* model) {
	this->model = model;
}

void Tower::Initialize() {
	worldTrans.Initialize();
	worldTrans.translation_ = { 0,0,-25 };
	worldTrans.TransferMatrix();
	touchableObject.reset(TouchableObject::Create(model, worldTrans, COLLISION_ATTR_LANDSHAPE));

	isDead = false;
	hp = 10000;
}

void Tower::Update() {
	if (hp < 0) {
		isDead = true;
	}

	if (touchableObject->GetCollider()->GetHitEnemyAttack()) {
 		hp -= 10;
		touchableObject->GetCollider()->Reset();
	}
}

void Tower::Draw(ViewProjection& viewProjection_) {
	if (isDead == false) {
		model->Draw(worldTrans, viewProjection_);
	}
}