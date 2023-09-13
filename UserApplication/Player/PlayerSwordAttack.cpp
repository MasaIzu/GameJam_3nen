#include"PlayerSwordAttack.h"
#include"PlayerNormal.h"
#include"Player.h"
#include <SphereCollider.h>
#include <CollisionAttribute.h>

int PlayerSwordAttack::overheatTimer = 0;
int PlayerSwordAttack::overheatLimit = 90;
bool PlayerSwordAttack::isOverheat = false;
int PlayerSwordAttack::energyCost = 100;

void PlayerSwordAttack::Initialize() {
	PlayerState::Initialize();
	canMove_ = false;
	action = Action::Antic;

	///�\������
	anticTime = 15;
	anticDistance = 7.0f;
	anticSpeed = anticDistance / static_cast<float>(anticTime);

	///�U��
	attackTime = 10;
	attackDistance = 3.0f;
	attackSpeed = attackDistance / static_cast<float>(attackTime);

	///�㌄
	afterTime = 0;

	//�����蔻��
	//�U������o�O�̉\������
	swordPos = { -1,5,0 };

	Radius = 1.0f;
	collider = new SphereCollider(Vector4(0, Radius, 0, 0), Radius);
	CollisionManager::GetInstance()->AddCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void PlayerSwordAttack::Update(Player* player, WorldTransform* worldTransform) {
	timer++;
	Move(worldTransform);
	switch (action) {
	case Action::Antic://�\������
		if (timer > anticTime) {
			timer = 0;
			collider->Reset();
			action = Action::Attack;
		}
		break;
	case Action::Attack://�U��
		/*collisionTransform.translation_ = swordPos * worldTransform->matWorld_;
		collisionTransform.TransferMatrix();*/
		collider->Update(worldTransform->matWorld_);

		if (timer > attackTime) {
			timer = 0;
			action = Action::After;
		}
		break;
	case Action::After://�㌄
		if (timer > afterTime) {
			isOverheat = true;
			overheatTimer = 0;
			CollisionManager::GetInstance()->RemoveCollider(collider);
			player->TransitionTo(new PlayerNormal);
		}
		break;
	}
}

void PlayerSwordAttack::Move(WorldTransform* worldTransform) {
	Vector3 moveVec = targetPos - worldTransform->translation_;
	moveVec.normalize();
	switch (action) {
	case Action::Antic://�\������
		worldTransform->translation_ += moveVec * anticSpeed;
		break;
	case Action::Attack://�U��
		worldTransform->translation_ += moveVec * attackSpeed;
		break;
	case Action::After://�㌄

		break;
	}
}

bool PlayerSwordAttack::CanSwordAttack() {
	if (isOverheat) {
		return false;
	}
	return  true;
}

void PlayerSwordAttack::StaticUpdate() {
	if (overheatTimer < overheatLimit) {
		overheatTimer++;
	}else {
		isOverheat = false;
	}
}