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
	//�a��
	if (input_->MouseInputTrigger(1)) {
		if (PlayerSwordAttack::CanSwordAttack()) {//����̃N�[���^�C���`�F�b�N
			if (player->UseEnergy(PlayerSwordAttack::GetEnergyCost())) {//�G�l���M�[�`�F�b�N
				player->TransitionTo(new PlayerSwordAttack);
			}
		}
	}else if (input_->MouseInputTrigger(0)) {
		player->TransitionTo(new PlayerShooting);
	}
}