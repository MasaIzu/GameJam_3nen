#include "PlayerBullet.h"
#include <imgui.h>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

PlayerBullet::PlayerBullet(Model* model){
	model_ = model;
}

PlayerBullet::~PlayerBullet(){
}

void PlayerBullet::Initialize(Vector3 pos, Vector3 velocity){
	
	isDead = false;
	liveLimit = 30;
	liveTimer = 0;

	worldTrans.Initialize();
	worldTrans.translation_ = pos;
	worldTrans.TransferMatrix();

	velocity_ = velocity;

	radius = 1.0f;
	collider = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	CollisionManager::GetInstance()->AddCollider(collider);
	collider->Update(worldTrans.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_ATTACK);
}

void PlayerBullet::Update(){
	liveTimer++;
	if (liveTimer > liveLimit) {
		isDead = true;
	}
	if (collider->GetSphereMeshHit() || collider->GetHitEnemyBody()) {
		isDead = true;
	}


	if (isDead) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
	}

	worldTrans.translation_ += velocity_;
	worldTrans.TransferMatrix();
	collider->Update(worldTrans.matWorld_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection_){
	model_->Draw(worldTrans, viewProjection_);
}
