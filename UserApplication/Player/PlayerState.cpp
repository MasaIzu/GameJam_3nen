#include "PlayerState.h"

Vector3 PlayerState::targetPos;
bool PlayerState::isLockOn;

void PlayerState::Initialize() {
	input_ = Input::GetInstance();
}