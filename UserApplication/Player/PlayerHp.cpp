#include"PlayerHp.h"

PlayerHp::PlayerHp() {
}

PlayerHp::~PlayerHp() {
}

void PlayerHp::Initialize() {
	MaxHp = 1000;
	hp = MaxHp;
	isLive = true;
}

void PlayerHp::Damage(int damage) {
	hp -= damage;
	if (hp <= 0) {
		isLive = false;
	}
}

void PlayerHp::Heal(int heal) {
	hp += heal;
	if (hp > MaxHp) {
		hp = MaxHp;
	}
}