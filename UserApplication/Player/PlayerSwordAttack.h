#pragma once
#include"PlayerState.h"

//Œ•Œ‚
class PlayerSwordAttack : public PlayerState {
public:
	void Initialize()override;
	void Update(Player* player, WorldTransform* worldTransform) override;

	static bool CanSwordAttack();
	static void staticUpdate();
	static int GetEnergyCost() { return energyCost; };

private:
	void Move(WorldTransform* worldTransform);

public:
	static bool isOverheat;
	static int overheatTimer;
	static int overheatLimit;
	static int energyCost;

private:
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
	Vector3 anticDistance;
	Vector3 anticSpeed;

	///UŒ‚
	//ŠÔ
	int attackTime;
	//ˆÚ“®‹——£
	Vector3 attackDistance;
	Vector3 attackSpeed;

	///ŒãŒ„
	//ŠÔ
	int afterTime;
};