#pragma once
#include"PlayerState.h"

//�ҋ@
class PlayerNormal : public PlayerState {
public:
	void Update(Player* player, WorldTransform* worldTransform) override;

private:

};