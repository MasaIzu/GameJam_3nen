#include"PlayerNormal.h"
#include"Player.h"

#include"PlayerSwordAttack.h"
#include"PlayerShooting.h"

void PlayerNormal::Initialize() {
	PlayerState::Initialize();
	canMove_ = true;
}

void PlayerNormal::Update(Player* player, WorldTransform* worldTransform) {
	//�a��
	if (input_->TriggerKey(DIK_1)) {
		if (PlayerSwordAttack::CanSwordAttack()) {//����̃N�[���^�C���`�F�b�N
			if (player->UseEnergy(PlayerSwordAttack::GetEnergyCost())) {//�G�l���M�[�`�F�b�N
				player->TransitionTo(new PlayerSwordAttack);
			}
		}
	}else if (input_->TriggerKey(DIK_2)) {
		player->TransitionTo(new PlayerShooting);
	}
}