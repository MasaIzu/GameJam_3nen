#pragma once
#include"ViewProjection.h"
#include"Sprite.h"

class PlayerEnergy {
public:
	void Initialize(int maxEnergy);
	void Update(int recoveryAmount);
	bool Use(int cost);

	void DrawSprite();
private:
	void recovery(int amount);

private:
	bool isUse;
	int energy_;
	int maxEnergy_;

	int coolTimeLimt;
	int coolTimer;

	bool isOverHeat;
	int overHeatTimer;
	int overHeatLimt;

	Vector2 spritePos;
	Vector2 gaugePos;
	Vector4 color;
	std::unique_ptr<Sprite> flameSprite;
	std::unique_ptr<Sprite> gaugeSprite;
	std::unique_ptr<Sprite> overHeatSprite;
};