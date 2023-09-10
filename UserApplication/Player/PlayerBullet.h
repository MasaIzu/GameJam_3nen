#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include <BaseCollider.h>
#include <CollisionManager.h>
#include "ParticleManager.h"
#include "Uint32Vector2.h"

class PlayerBullet {

public://基本関数
	PlayerBullet();
	~PlayerBullet();

	void Initialize(Vector3 pos, Vector3 velocity);
	void Update();
	void Draw(ViewProjection& viewProjection_);

	bool IsDead() { return isDead; };
	
private://クラス関連
	std::unique_ptr<Model> model_;//静的かなんかに変更する
	WorldTransform worldTrans;
	BaseCollider* collider = nullptr;

private://クラス変数
	bool isDead;
	int liveTimer;
	int liveLimit;
	bool isExpanding = false;
	bool isMovingExpandingBullet = false;

	float radius;

	Vector3 velocity_;//移動量
	Vector3 BulletVector;//打ち出される方向
};