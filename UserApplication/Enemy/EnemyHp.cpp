#include"EnemyHp.h"

EnemyHp::EnemyHp() {
}

EnemyHp::~EnemyHp() {
}

void EnemyHp::Initialize() {
	MaxHp = 1;
	hp = MaxHp;
	isLive = true;
}

void EnemyHp::Damage(int damage) {
	hp -= damage;
	if (hp <= 0) {
		isLive = false;
	}
}

void EnemyHp::Heal(int heal) {
	hp += heal;
	if (hp > MaxHp) {
		hp = MaxHp;
	}
}