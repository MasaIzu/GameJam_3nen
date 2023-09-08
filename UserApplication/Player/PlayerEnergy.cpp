#include"PlayerEnergy.h"

void PlayerEnergy::Initialize(int maxEnergy) {
	maxEnergy_ = maxEnergy;
	energy_ = maxEnergy;
	isUse = false;

	coolTimer = 0;
	coolTimeLimt = 60;

	isOverHeat = false;
	overHeatTimer = 0;
	overHeatLimt = 90;
}

void PlayerEnergy::Update(int recoveryAmount) {
	//通常
	if (isUse == false) {
		if (coolTimer < coolTimeLimt) {
			coolTimer++;
		};
	}

	//オーバーヒート
	if (isOverHeat) {
		overHeatTimer++;
		if (overHeatTimer >= overHeatLimt) {
			isOverHeat = false;
		}
	}

	//回復
	if (coolTimer >= coolTimeLimt && overHeatTimer >= overHeatLimt) {
		recovery(recoveryAmount);
	}

	isUse = false;
}

bool PlayerEnergy::Use(int cost) {
	if (energy_ > 0) {
		energy_ -= cost;
		isUse = true;
		coolTimer = 0;
		if (energy_ <= 0) {
			energy_ = 0;
			isOverHeat = true;
			overHeatTimer = 0;
		}
		return true;
	}
	return false;
}

void PlayerEnergy::recovery(int amount) {
	energy_ += amount;
	if (energy_ > maxEnergy_) {
		energy_ = maxEnergy_;
	}
}