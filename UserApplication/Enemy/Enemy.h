#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include <Sprite.h>
#include "SplinePosition.h"

#include "EnemyState.h"
#include "EnemyHp.h"
#include "EnemyBullet.h"

class Enemy {
public://��{�֐�
	Enemy();
	~Enemy();

	void Initialize(const Vector3& Pos);
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite();

	void OnCollision();
	bool IsDead() { return isDead; };

	//��Ԉڍs
	void TransitionTo(EnemyState* state);

	//�p�[�e�B�N�����o���p
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

	//�e����
	void CreatBullet(Vector3 pos, Vector3 velocity);
private:
	//����
	void Fall();
	//�ړ��̒l�X�V
	void WorldTransUpdate();
	//�����蔻��
	void CheckEnemyCollider();

public://Setter
	void SetPlayerPos(Vector3 playerPos) { this->playerPos = playerPos; };

public://Getter
	Vector3 GetEnemyPos()const { return MyMath::GetWorldTransform(enemyWorldTrans.matWorld_); }

private://�N���X�֘A
	Input* input = nullptr;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> bulletModel_;
	WorldTransform enemyWorldTrans;
	BaseCollider* enemyCollider = nullptr;
	EnemyHp hp;
	EnemyState* state_;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
private://�C�[�i���N���X

private://�ʃN���X����l�����炤
	Vector3 towerPos;

private://�N���X�ϐ�
	bool isDead;
	float Radius;
	float coliisionHeight;
	bool onGround;
	Vector3 playerPos;
	
	//�ړ�
	Vector3 enemyOldPos;

	//�ˌ�
	int timer;
	int coolTime;
	float bulletSpeed;

	//�W�����v
	bool isJump;
	float jumpSpeed;
	int jumpTimer;
	int jumpLimit;
	float ascendSpeed;
	int ascendCost;

	//����
	float fallSpeed;

	//hp
	int healAmount;
	int healNum;
};