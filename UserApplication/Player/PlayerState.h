#pragma once
#include "WorldTransform.h"
#include "Input.h"

class Player;

class PlayerState {
public:
	virtual void Update(Player* player, WorldTransform* worldTransform) = 0;

protected:

protected:

private:

};