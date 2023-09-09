#include"PlayerNormal.h"
#include"Player.h"

#include"PlayerSwordAttack.h"
#include"PlayerShooting.h"

void PlayerNormal::Initialize() {
	PlayerState::Initialize();
	canMove_ = true;
}

void PlayerNormal::Update(Player* player, WorldTransform* worldTransform) {
	//斬撃
	if (input_->TriggerKey(DIK_1)) {
		if (PlayerSwordAttack::CanSwordAttack()) {//武器のクールタイムチェック
			if (player->UseEnergy(PlayerSwordAttack::GetEnergyCost())) {//エネルギーチェック
				player->TransitionTo(new PlayerSwordAttack);
			}
		}
	}else if (input_->TriggerKey(DIK_2)) {
		player->TransitionTo(new PlayerShooting);
	}
}