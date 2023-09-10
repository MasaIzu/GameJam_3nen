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
	//’Êíó‘Ô‚É–ß‚é
	if (input_->MouseOffTrigger(0)) {
		player->TransitionTo(new PlayerNormal);
	}
	
	//’e”­ŽË
	if (isReload == false) {
		timer++;
		if (timer > limit) {
			magazine--;
			Vector3 velocity = worldTransform->LookVelocity.look * bulletSpeed;
			Vector3 gunPos = { 2,5,0 };
			Vector3 createPos = gunPos * worldTransform->matWorld_;
			player->CreatBullet(createPos, velocity);
			timer = 0;
		}
	}

	//Žc’e0
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