#include"PlayerEnergy.h"
#include"TextureManager.h"
#include"WinApp.h"

void PlayerEnergy::Initialize(int maxEnergy) {
	maxEnergy_ = maxEnergy;
	energy_ = maxEnergy;
	isUse = false;

	coolTimer = 0;
	coolTimeLimt = 60;

	isOverHeat = false;
	overHeatTimer = 0;
	overHeatLimt = 90;

	spritePos = { WinApp::window_width / 2.0f, 4.0f * WinApp::window_height / 5.0f };
	flameSprite = Sprite::Create(TextureManager::Load("sprite/energyFlame.png"));
	flameSprite->SetAnchorPoint({ 0.5f,0.5f });

	gaugeSprite = Sprite::Create(TextureManager::Load("white1x1.png"));
	gaugeSprite->SetAnchorPoint({ 0.0f,0.5f });
	gaugePos = spritePos;
	gaugePos.x -= flameSprite->GetSize().x / 2.0f;

	overHeatSprite = Sprite::Create(TextureManager::Load("white1x1.png"));
	overHeatSprite->SetAnchorPoint({ 0.5f,0.5f });
	overHeatSprite->SetSize({ 398.0f,6.0f });
	color = { 1,0,0,0 };
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
		color = { 1,0,0,1 };
		if (overHeatTimer >= overHeatLimt) {
			isOverHeat = false;
			color = { 1,0,0,0 };
		}
	}

	//回復
	if (coolTimer >= coolTimeLimt && isOverHeat == false) {
		recovery(recoveryAmount);
	}

	
	gaugeSprite->SetSize({ static_cast<float>(energy_) * flameSprite->GetSize().x / maxEnergy_ , 6.0f });

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

void PlayerEnergy::DrawSprite() {
	flameSprite->Draw(spritePos, { 1,1,1,1 });
	gaugeSprite->Draw(gaugePos, { 1,1,1,1 });
	overHeatSprite->Draw(spritePos, color);
}