#pragma once
#include "WorldTransform.h"
#include <BaseCollider.h>
#include"PlayerState.h"

//Œ•Œ‚
class PlayerSwordAttack : public PlayerState {
public:
	void Initialize()override;
	void Update(Player* player, WorldTransform* worldTransform, WorldTransform* swordTrans) override;

	static bool CanSwordAttack();
	static void StaticUpdate();
	static int GetEnergyCost() { return energyCost; };

private:
	void Move(WorldTransform* worldTransform);

public:
	static bool isOverheat;
	static int overheatTimer;
	static int overheatLimit;
	static int energyCost;

private:
	BaseCollider* collider = nullptr;
	float Radius;

	enum class Action {
		Antic,
		Attack,
		After,
	};
	Action action;
	int timer;

	///—\”õ“®ì
	//ŠÔ
	int anticTime;
	//ˆÚ“®‹——£
	float anticDistance;
	float anticSpeed;

	///UŒ‚
	//ŠÔ
	int attackTime;
	//ˆÚ“®‹——£
	float attackDistance;
	float attackSpeed;
	Vector3 swordPos;

	///ŒãŒ„
	//ŠÔ
	int afterTime;
};