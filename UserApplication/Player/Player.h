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

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

private:
	//プレーヤーの移動
	void Move();
	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	// プレイヤーの当たり判定
	void CheckPlayerCollider();

public://Setter

public://Getter

private://クラス関連
	Input* input = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	PlayerState* state_;
private://イーナムクラス


private://別クラスから値をもらう


private://プレイヤークラス変数
	//移動速度
	float straightSpeed;
	float diagonalSpeed;
};