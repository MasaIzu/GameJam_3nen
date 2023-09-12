#include "Enemy.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
//#include <FbxLoader.h>
#include <SphereCollider.h>

#include "EnemyApproach.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(const Vector3& Pos) {
	input = Input::GetInstance();

	enemyWorldTrans.Initialize();
	enemyWorldTrans.translation_ = Pos;
	model_.reset(Model::CreateFromOBJ("sphere", true));
	bulletModel_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));

	// コリジョンマネージャに追加
	Radius = 1.0f;
	enemyCollider = new SphereCollider(Vector4(0, Radius, 0, 0), Radius);
	CollisionManager::GetInstance()->AddCollider(enemyCollider);
	enemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	enemyCollider->Update(enemyWorldTrans.matWorld_);
	coliisionHeight = 1.0f;
	onGround = false;

	hp.Initialize();
	healAmount = 500;
	healNum = 3;
	isDead = false;

	state_ = new EnemyApproach;
	state_->Initialize();

	//ジャンプ
	isJump = false;
	jumpSpeed = 0.6f;
	jumpTimer = 0;
	jumpLimit = 20;

	ascendSpeed = 0.3f;
	ascendCost = 5;

	fallSpeed = -0.2f;
}

void Enemy::Update() {
	enemyOldPos = enemyWorldTrans.translation_;

	state_->Update(this, &enemyWorldTrans);

	//弾更新
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->IsDead(); });
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}

	CheckEnemyCollider();
	WorldTransUpdate();

	OnCollision();
	if (hp.IsLive() == false) {
		isDead = true;
		CollisionManager::GetInstance()->RemoveCollider(enemyCollider);
	}
}

void Enemy::Draw(ViewProjection& viewProjection_) {
	model_->Draw(enemyWorldTrans, viewProjection_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw(viewProjection_);
	}
}

void Enemy::DrawSprite() {

}

void Enemy::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{

}

void Enemy::ParticleDraw(ViewProjection& viewProjection_)
{

}

void Enemy::CopyParticle()
{

}

void Enemy::Fall() {
	// 移動
	enemyWorldTrans.translation_.y += fallSpeed;
}

void Enemy::WorldTransUpdate() {
	if (enemyWorldTrans.translation_.y < -15) {
		enemyWorldTrans.translation_ = { 0,0,0 };
	}
	enemyWorldTrans.TransferMatrix();
}

void Enemy::CreatBullet(Vector3 pos, Vector3 velocity) {
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>(bulletModel_.get());
	newBullet->Initialize(pos, velocity);
	bullets.push_back(std::move(newBullet));
}

//状態変更
void Enemy::TransitionTo(EnemyState* state) {
	//削除
	delete state_;
	//新規作成
	state_ = state;
	state_->Initialize();
}

void Enemy::OnCollision() {
	if (enemyCollider->GetHitPlayerAttack()) {
		hp.Damage(100);
		enemyCollider->Reset();
	}
}

void Enemy::CheckEnemyCollider() {
	enemyCollider->Update(enemyWorldTrans.matWorld_);
	Vector3 moveMent = enemyWorldTrans.translation_ - enemyOldPos;
	moveMent = { abs(moveMent.x),abs(moveMent.y) ,abs(moveMent.z) };

	//地面メッシュコライダー
	{
		// 球の上端から球の下端までのレイキャスト
		Ray Groundray;
		Groundray.start = MyMath::Vec3ToVec4(enemyOldPos);
		Groundray.start.y += Radius;
		Groundray.dir = { 0,-1.0f,0,0 };
		RaycastHit raycastHit;

		// 接地状態
		if (onGround) {
			// スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			// 接地を維持
			if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, Radius + adsDistance + moveMent.y)) {
				onGround = true;
				enemyWorldTrans.translation_.y -= raycastHit.distance - (Radius + moveMent.y);
				enemyOldPos.y = enemyWorldTrans.translation_.y;
			}
			// 地面がないので落下
			else {
				onGround = false;
			}
		}
		// 落下状態
		else {
			if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, Radius + moveMent.y)) {
				// 着地
				onGround = true;
				enemyWorldTrans.translation_.y -= raycastHit.distance - (Radius + moveMent.y);
				enemyOldPos.y = enemyWorldTrans.translation_.y;
			}
		}
	}

	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(enemyOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 0,0,1,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.z)) {
			enemyWorldTrans.translation_.z -= (moveMent.z - wallRaycastHit.distance) + Radius;
			enemyOldPos.z = enemyWorldTrans.translation_.z;
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(enemyOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 0,0,-1,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.z)) {
			enemyWorldTrans.translation_.z += (moveMent.z - wallRaycastHit.distance) + Radius;
			enemyOldPos.z = enemyWorldTrans.translation_.z;
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(enemyOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.x)) {
			enemyWorldTrans.translation_.x -= (moveMent.x - wallRaycastHit.distance) + Radius;
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(enemyOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { -1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.x)) {
			enemyWorldTrans.translation_.x += (moveMent.x - wallRaycastHit.distance) + Radius;
		}
	}
}