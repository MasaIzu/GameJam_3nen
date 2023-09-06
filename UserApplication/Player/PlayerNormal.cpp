#include"PlayerNormal.h"
#include"Player.h"

#include"PlayerSwordAttack.h"

void PlayerNormal::Initialize() {
	PlayerState::Initialize();
	canMove_ = true;
}

void PlayerNormal::Update(Player* player, WorldTransform* worldTransform) {
	if (input_->TriggerKey(DIK_1)) {
		player->TransitionTo(new PlayerSwordAttack);
	}
}