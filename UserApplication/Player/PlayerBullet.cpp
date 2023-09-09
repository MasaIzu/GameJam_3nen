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
}

void PlayerBullet::Update(){
	liveTimer++;
	if (liveTimer > liveLimit) {
		isDead = true;
	}

	worldTrans.translation_ += velocity_;
	worldTrans.TransferMatrix();
}

void PlayerBullet::Draw(ViewProjection& viewProjection_){
	model_->Draw(worldTrans, viewProjection_);
}