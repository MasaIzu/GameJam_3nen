#pragma once
#include"PlayerState.h"

class PlayerShooting : public PlayerState {
public:
	void Initialize()override;
	void Update(Player* player, WorldTransform* worldTransform, WorldTransform* swordTrans) override;

	static void Reload();
	static void StaticUpdate();

private:
	static int magazine;
	static int magazineMax;

	static int reloadTimer;
	static int reloadLimit;
	static bool isReload;

private:
	//クールタイム
	int timer;
	int limit;

	float bulletSpeed;
};