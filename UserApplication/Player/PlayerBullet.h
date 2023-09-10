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

public://��{�֐�
	PlayerBullet();
	~PlayerBullet();

	void Initialize(Vector3 pos, Vector3 velocity);
	void Update();
	void Draw(ViewProjection& viewProjection_);

	bool IsDead() { return isDead; };
	
private://�N���X�֘A
	std::unique_ptr<Model> model_;//�ÓI���Ȃ񂩂ɕύX����
	WorldTransform worldTrans;
	BaseCollider* collider = nullptr;

private://�N���X�ϐ�
	bool isDead;
	int liveTimer;
	int liveLimit;
	bool isExpanding = false;
	bool isMovingExpandingBullet = false;

	float radius;

	Vector3 velocity_;//�ړ���
	Vector3 BulletVector;//�ł��o��������
};