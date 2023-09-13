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
	//Å‘åHP
	int MaxHp;
	//Œ»İ‚ÌHP
	int hp;

	//¶‘¶ƒtƒ‰ƒO
	bool isLive;
};