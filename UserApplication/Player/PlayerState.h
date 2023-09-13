#pragma once
#include "WorldTransform.h"
#include "Input.h"

class Player;

class PlayerState {
public:
	virtual void Initialize();
	std::string GetStateName() { return stateName; };
	virtual void Update(Player* player, WorldTransform* worldTransform, WorldTransform* swordTrans) = 0;


	static void SettargetPos(Vector3 TargetPos) { targetPos = TargetPos; };
	static void SetIsLockOn(bool IsLockOn) { isLockOn = IsLockOn; };
	static bool GetIsLockOn() { return isLockOn; };
public:
	bool CanMove() { return canMove_; };

protected:
	Input* input_ = nullptr;
	bool canMove_;
	static Vector3 targetPos;
	static bool isLockOn;
	std::string stateName;
};