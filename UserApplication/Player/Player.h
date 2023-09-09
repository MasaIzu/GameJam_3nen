#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <Sprite.h>
#include "SplinePosition.h"

#include "PlayerState.h"
#include "PlayerEnergy.h"

class Player {

public://基本関数
	Player();
	~Player();

	void Initialize(const Vector3& Pos, ViewProjection* viewProjection);
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite(ViewProjection& viewProjection_);

	//状態移行
	void TransitionTo(PlayerState* state);
	//エネルギー
	bool UseEnergy(int cost) { return energy.Use(cost); };

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

private:
	//プレーヤーの移動
	void Move();
	//プレイヤーの回転
	void PlayerRot();
	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	// プレイヤーの当たり判定
	void CheckPlayerCollider();

public://Setter
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }
	
public://Getter
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }

private://クラス関連
	Input* input = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	PlayerState* state_;
	PlayerEnergy energy;
private://イーナムクラス


private://別クラスから値をもらう
	Vector2 cameraRot;

private://プレイヤークラス変数
	//移動
	float straightSpeed;
	float diagonalSpeed;

	bool isBoost;
	int QuickBoostCost;

	int boostCost;
	int boostTimer;
	int boostChangeTime;

	int energyRecoveryAmount;
};