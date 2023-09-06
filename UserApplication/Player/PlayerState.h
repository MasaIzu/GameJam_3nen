#pragma once
#include "WorldTransform.h"
#include "Input.h"

class Player;

class PlayerState {
public:
	virtual void Initialize();
	virtual void Update(Player* player, WorldTransform* worldTransform) = 0;

public:
	bool CanMove() { return canMove_; };

protected:
	Input* input_ = nullptr;
	bool canMove_;
};