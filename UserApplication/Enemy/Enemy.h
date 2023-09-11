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

#include "EnemyHp.h"
#include "EnemyBullet.h"

class Enemy {
public://基本関数
	Enemy();
	~Enemy();

	void Initialize(const Vector3& Pos, ViewProjection* viewProjection);
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite();

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

	//弾生成
	void CreatBullet(Vector3 pos, Vector3 velocity);
private:
	//プレーヤーの移動
	void Move();
	//プレイヤーのジャンプ
	void Jump();
	//落下
	void Fall();
	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	// プレイヤーの当たり判定
	void CheckEnemyCollider();

public://Setter
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }

public://Getter
	Vector3 GetEnemyPos()const { return MyMath::GetWorldTransform(enemyWorldTrans.matWorld_); }

private://クラス関連
	Input* input = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform enemyWorldTrans;
	BaseCollider* enemyCollider = nullptr;
	EnemyHp hp;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	// コライダー
private://イーナムクラス

private://別クラスから値をもらう
	Vector2 cameraRot;

private://プレイヤークラス変数
	bool isDead;
	float Radius;
	float coliisionHeight;
	bool onGround;
	//移動
	Vector3 enemyOldPos;
	float straightSpeed;
	float diagonalSpeed;
	bool isBoost;
	int QuickBoostCost;
	int boostCost;
	int boostTimer;
	int boostChangeTime;

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