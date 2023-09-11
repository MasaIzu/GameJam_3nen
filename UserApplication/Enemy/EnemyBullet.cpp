#include "EnemyBullet.h"
#include <imgui.h>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

EnemyBullet::EnemyBullet() {
}

EnemyBullet::~EnemyBullet() {
}

void EnemyBullet::Initialize(Vector3 pos, Vector3 velocity) {

	isDead = false;
	liveLimit = 30;
	liveTimer = 0;
	model_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));

	worldTrans.Initialize();
	worldTrans.translation_ = pos;
	worldTrans.TransferMatrix();

	velocity_ = velocity;

	radius = 1.0f;
	collider = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	CollisionManager::GetInstance()->AddCollider(collider);
	collider->Update(worldTrans.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_ENEMYBULLETATTACK);
}

void EnemyBullet::Update() {
	liveTimer++;
	if (liveTimer > liveLimit) {
		isDead = true;
	}
	if (collider->GetSphereMeshHit()) {
		isDead = true;
	}
	if (isDead) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
	}

	worldTrans.translation_ += velocity_;
	worldTrans.TransferMatrix();
	collider->Update(worldTrans.matWorld_);
}

void EnemyBullet::Draw(ViewProjection& viewProjection_) {
	model_->Draw(worldTrans, viewProjection_);
}