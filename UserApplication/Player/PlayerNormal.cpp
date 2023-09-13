#include"PlayerNormal.h"
#include"Player.h"

#include"PlayerSwordAttack.h"
#include"PlayerShooting.h"

void PlayerNormal::Initialize() {
	PlayerState::Initialize();
	canMove_ = true;
	stateName = "NORMAL";
}

void PlayerNormal::Update(Player* player, WorldTransform* worldTransform, WorldTransform* swordTrans) {
	//斬撃
	if (input_->MouseInputTrigger(1)) {
		if (PlayerSwordAttack::CanSwordAttack()) {//武器のクールタイムチェック
			if (player->UseEnergy(PlayerSwordAttack::GetEnergyCost())) {//エネルギーチェック
				player->TransitionTo(new PlayerSwordAttack);
			}
		}
	}else if (input_->MouseInputTrigger(0)) {
		player->TransitionTo(new PlayerShooting);
	}
}