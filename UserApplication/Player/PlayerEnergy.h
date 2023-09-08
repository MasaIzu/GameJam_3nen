#pragma once

class PlayerEnergy {
public:
	void Initialize(int maxEnergy);
	void Update(int recoveryAmount);
	bool Use(int cost);
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
};