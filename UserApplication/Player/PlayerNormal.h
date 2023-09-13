#pragma once
#include"PlayerState.h"

//‘Ò‹@
class PlayerNormal : public PlayerState {
public:
	void Initialize()override;
	void Update(Player* player, WorldTransform* worldTransform, WorldTransform* swordTrans) override;

private:

};