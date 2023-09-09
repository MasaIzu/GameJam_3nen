#pragma once
#include"PlayerState.h"

//����
class PlayerSwordAttack : public PlayerState {
public:
	void Initialize()override;
	void Update(Player* player, WorldTransform* worldTransform) override;

	static bool CanSwordAttack();
	static void staticUpdate();
	static int GetEnergyCost() { return energyCost; };

private:
	void Move(WorldTransform* worldTransform);

public:
	static bool isOverheat;
	static int overheatTimer;
	static int overheatLimit;
	static int energyCost;

private:
	enum class Action {
		Antic,
		Attack,
		After,
	};
	Action action;
	int timer;

	///�\������
	//����
	int anticTime;
	//�ړ�����
	float anticDistance;
	float anticSpeed;

	///�U��
	//����
	int attackTime;
	//�ړ�����
	float attackDistance;
	float attackSpeed;

	///�㌄
	//����
	int afterTime;
};