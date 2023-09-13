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

	///予備動作
	anticTime = 15;
	anticDistance = 7.0f;
	anticSpeed = anticDistance / static_cast<float>(anticTime);

	///攻撃
	attackTime = 10;
	attackDistance = 3.0f;
	attackSpeed = attackDistance / static_cast<float>(attackTime);

	///後隙
	afterTime = 0;

	//当たり判定
	//攻撃判定バグの可能性あり
	swordPos = { -1,5,0 };

	Radius = 10.0f;
	collider = new SphereCollider(Vector4(0, Radius, 0, 0), Radius);
	CollisionManager::GetInstance()->AddCollider(collider);
	stateName = "SWORD";
	collider->SetAttribute(COLLISION_ATTR_ATTACK);
}

void PlayerSwordAttack::Update(Player* player, WorldTransform* worldTransform, WorldTransform* swordTrans) {
	timer++;
	Move(worldTransform);
	switch (action) {
	case Action::Antic://予備動作
		if (timer > anticTime) {
			timer = 0;
			collider->Reset();
			action = Action::Attack;
		}
		break;
	case Action::Attack://攻撃
		collider->Update(swordTrans->matWorld_);

		if (timer > attackTime) {
			timer = 0;
			action = Action::After;
		}
		break;
	case Action::After://後隙
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
	case Action::Antic://予備動作
		worldTransform->translation_ += moveVec * anticSpeed;
		break;
	case Action::Attack://攻撃
		worldTransform->translation_ += moveVec * attackSpeed;
		break;
	case Action::After://後隙

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