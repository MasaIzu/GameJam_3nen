#include"PlayerSwordAttack.h"
#include"PlayerNormal.h"
#include"Player.h"

void PlayerSwordAttack::Initialize() {
	PlayerState::Initialize();
	canMove_ = false;
	action = Action::Antic;

	///—\”õ“®ì
	anticTime = 15;
	anticDistance = { 0,0,0.5f };
	anticSpeed = anticDistance / static_cast<float>(anticTime);

	///UŒ‚
	attackTime = 10;
	attackDistance = { 0,0,1 };
	attackSpeed = attackDistance / static_cast<float>(attackTime);

	///ŒãŒ„
	afterTime = 5;
}

void PlayerSwordAttack::Update(Player* player, WorldTransform* worldTransform) {
	timer++;
	Move(worldTransform);
	switch (action) {
	case Action::Antic://—\”õ“®ì
		if (timer > anticTime) {
			timer = 0;
			action = Action::Attack;
		}
		break;
	case Action::Attack://UŒ‚
		if (timer > attackTime) {
			timer = 0;
			action = Action::After;
		}
		break;
	case Action::After://ŒãŒ„
		if (timer > afterTime) {
			player->TransitionTo(new PlayerNormal);
		}
		break;
	}
}

void PlayerSwordAttack::Move(WorldTransform* worldTransform) {
	switch (action) {
	case Action::Antic://—\”õ“®ì
		worldTransform->translation_ += worldTransform->LookVelocity.look * anticSpeed;
		break;
	case Action::Attack://UŒ‚
		worldTransform->translation_ += worldTransform->LookVelocity.look * attackSpeed;
		break;
	case Action::After://ŒãŒ„

		break;
	}
}