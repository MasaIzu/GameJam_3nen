#include"PlayerSwordAttack.h"
#include"PlayerNormal.h"
#include"Player.h"

int PlayerSwordAttack::overheatTimer = 0;
int PlayerSwordAttack::overheatLimit = 90;
bool PlayerSwordAttack::isOverheat = false;
int PlayerSwordAttack::energyCost = 100;

void PlayerSwordAttack::Initialize() {
	PlayerState::Initialize();
	canMove_ = false;
	action = Action::Antic;

	///ó\îıìÆçÏ
	anticTime = 15;
	anticDistance = { 0,0,0.5f };
	anticSpeed = anticDistance / static_cast<float>(anticTime);

	///çUåÇ
	attackTime = 10;
	attackDistance = { 0,0,1 };
	attackSpeed = attackDistance / static_cast<float>(attackTime);

	///å„åÑ
	afterTime = 5;
}

void PlayerSwordAttack::Update(Player* player, WorldTransform* worldTransform) {
	timer++;
	Move(worldTransform);
	switch (action) {
	case Action::Antic://ó\îıìÆçÏ
		if (timer > anticTime) {
			timer = 0;
			action = Action::Attack;
		}
		break;
	case Action::Attack://çUåÇ
		if (timer > attackTime) {
			timer = 0;
			action = Action::After;
		}
		break;
	case Action::After://å„åÑ
		if (timer > afterTime) {
			isOverheat = true;
			overheatTimer = 0;
			player->TransitionTo(new PlayerNormal);
		}
		break;
	}
}

void PlayerSwordAttack::Move(WorldTransform* worldTransform) {
	switch (action) {
	case Action::Antic://ó\îıìÆçÏ
		worldTransform->translation_ += worldTransform->LookVelocity.look * anticSpeed;
		break;
	case Action::Attack://çUåÇ
		worldTransform->translation_ += worldTransform->LookVelocity.look * attackSpeed;
		break;
	case Action::After://å„åÑ

		break;
	}
}

bool PlayerSwordAttack::CanSwordAttack() {
	if (isOverheat) {
		return false;
	}
	return  true;
}

void PlayerSwordAttack::staticUpdate() {
	if (overheatTimer < overheatLimit) {
		overheatTimer++;
	}else {
		isOverheat = false;
	}
}