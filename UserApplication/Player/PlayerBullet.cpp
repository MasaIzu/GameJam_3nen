#include "PlayerBullet.h"
#include <imgui.h>
#include <CollisionManager.h>
#include <SphereCollider.h>
#include <CollisionAttribute.h>

PlayerBullet::PlayerBullet(){
}

PlayerBullet::~PlayerBullet(){
}

void PlayerBullet::Initialize(Vector3 pos, Vector3 velocity){
	
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
	collider->SetAttribute(COLLISION_ATTR_ATTACK);
}

void PlayerBullet::Update(){
	liveTimer++;
	if (liveTimer > liveLimit) {
		isDead = true;
	}
	if (collider->GetHit()) {
		isDead = true;
	}
	if (isDead) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
	}

	worldTrans.translation_ += velocity_;
	worldTrans.TransferMatrix();
	collider->Update(worldTrans.matWorld_);
}

void PlayerBullet::Draw(ViewProjection& viewProjection_){
	model_->Draw(worldTrans, viewProjection_);
}