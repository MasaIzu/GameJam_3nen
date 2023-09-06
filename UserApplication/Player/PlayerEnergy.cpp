#include"PlayerEnergy.h"

void PlayerEnergy::Initialize(int maxEnergy) {
	maxEnergy_ = maxEnergy;
	energy_ = maxEnergy;
	isUse = false;

	isOverHeat = false;
	overHeatTimer = 0;
	overHeatLimt = 90;
}

void PlayerEnergy::Update(int recoveryAmount) {
	if (isOverHeat) {
		overHeatTimer++;
		if (overHeatTimer >= overHeatLimt) {
			isOverHeat = false;
		}
	}else {
		if (isUse == false) {
			recovery(recoveryAmount);
		}
	}
	isUse = false;
}

bool PlayerEnergy::Use(int cost) {
	if (energy_ > 0) {
		energy_ -= cost;
		isUse = true;
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