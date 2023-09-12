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
public://基本関数
	Enemy();
	~Enemy();

	void Initialize(const Vector3& Pos);
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite();

	void OnCollision();
	bool IsDead() { return isDead; };

	//状態移行
	void TransitionTo(EnemyState* state);

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

	//弾生成
	void CreatBullet(Vector3 pos, Vector3 velocity);
private:
	//落下
	void Fall();
	//移動の値更新
	void WorldTransUpdate();
	//当たり判定
	void CheckEnemyCollider();

public://Setter
	void SetPlayerPos(Vector3 playerPos) { this->playerPos = playerPos; };

public://Getter
	Vector3 GetEnemyPos()const { return MyMath::GetWorldTransform(enemyWorldTrans.matWorld_); }

private://クラス関連
	Input* input = nullptr;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> bulletModel_;
	WorldTransform enemyWorldTrans;
	BaseCollider* enemyCollider = nullptr;
	EnemyHp hp;
	EnemyState* state_;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
private://イーナムクラス

private://別クラスから値をもらう
	Vector3 towerPos;

private://クラス変数
	bool isDead;
	float Radius;
	float coliisionHeight;
	bool onGround;
	Vector3 playerPos;
	
	//移動
	Vector3 enemyOldPos;

	//射撃
	int timer;
	int coolTime;
	float bulletSpeed;

	//ジャンプ
	bool isJump;
	float jumpSpeed;
	int jumpTimer;
	int jumpLimit;
	float ascendSpeed;
	int ascendCost;

	//落下
	float fallSpeed;

	//hp
	int healAmount;
	int healNum;
};