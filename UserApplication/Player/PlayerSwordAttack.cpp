#include"PlayerSwordAttack.h"
#include"PlayerNormal.h"
#include"Player.h"

void PlayerSwordAttack::Initialize() {
	PlayerState::Initialize();
	canMove_ = false;
	action = Action::Antic;

	///�\������
	anticTime = 15;
	anticDistance = { 0,0,0.5f };
	anticSpeed = anticDistance / static_cast<float>(anticTime);

	///�U��
	attackTime = 10;
	attackDistance = { 0,0,1 };
	attackSpeed = attackDistance / static_cast<float>(attackTime);

	///�㌄
	afterTime = 5;
}

void PlayerSwordAttack::Update(Player* player, WorldTransform* worldTransform) {
	timer++;
	Move(worldTransform);
	switch (action) {
	case Action::Antic://�\������
		if (timer > anticTime) {
			timer = 0;
			action = Action::Attack;
		}
		break;
	case Action::Attack://�U��
		if (timer > attackTime) {
			timer = 0;
			action = Action::After;
		}
		break;
	case Action::After://�㌄
		if (timer > afterTime) {
			player->TransitionTo(new PlayerNormal);
		}
		break;
	}
}

void PlayerSwordAttack::Move(WorldTransform* worldTransform) {
	switch (action) {
	case Action::Antic://�\������
		worldTransform->translation_ += worldTransform->LookVelocity.look * anticSpeed;
		break;
	case Action::Attack://�U��
		worldTransform->translation_ += worldTransform->LookVelocity.look * attackSpeed;
		break;
	case Action::After://�㌄

		break;
	}
}