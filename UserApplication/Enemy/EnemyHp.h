#pragma once

class EnemyHp {
public:
	EnemyHp();
	~EnemyHp();

	void Initialize();

	int GetHp() { return hp; };
	bool IsLive() { return isLive; };

	void Damage(int damage);
	void Heal(int heal);

private:
	//最大HP
	int MaxHp;
	//現在のHP
	int hp;

	//生存フラグ
	bool isLive;
};