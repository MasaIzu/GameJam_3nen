#include"PlayerShooting.h"
#include"PlayerNormal.h"
#include"Player.h"

#include"ImGuiManager.h"

int PlayerShooting::magazineMax = 30;
int PlayerShooting::magazine = magazineMax;
int PlayerShooting::reloadTimer = 0;;
int PlayerShooting::reloadLimit = 90;
bool PlayerShooting::isReload = false;

void PlayerShooting::Initialize() {
	PlayerState::Initialize();
	timer = 0;
	limit = 10;
	canMove_ = true;
	bulletSpeed = 2.0f;
}

void PlayerShooting::Update(Player* player, WorldTransform* worldTransform) {
	//通常状態に戻る
	if (input_->MouseOffTrigger(0)) {
		player->TransitionTo(new PlayerNormal);
	}
	
	//弾発射
	if (isReload == false) {
		timer++;
		if (timer > limit) {
			magazine--;
			Vector3 gunPos = { 2,5,0 };
			Vector3 createPos = gunPos * worldTransform->matWorld_;

			Vector3 velocity;

			if (isLockOn) {
				Vector3 moveVec = targetPos - createPos;
				moveVec.normalize();
				velocity = moveVec * bulletSpeed;
			}else {
				velocity = worldTransform->LookVelocity.look * bulletSpeed;
			}

			player->CreatBullet(createPos, velocity);
			timer = 0;
		}
	}

	//残弾0
	if (magazine <= 0) {
		Reload();
	}

	ImGui::Begin("shooting");
	ImGui::Text("magazin : %d", magazine);
	ImGui::Text("reload: % d", isReload);
	ImGui::End();
}

void PlayerShooting::Reload() {
	if (isReload == false) {
		reloadTimer = 0;
		isReload = true;
	}
}

void PlayerShooting::StaticUpdate() {
	if (isReload) {
		reloadTimer++;
		if (reloadTimer > reloadLimit) {
			magazine = magazineMax;
			isReload = false;
		}
	}
}