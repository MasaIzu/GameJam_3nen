#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
//#include <FbxLoader.h>
#include <SphereCollider.h>

#include"PlayerNormal.h"
#include"PlayerSwordAttack.h"
#include"PlayerShooting.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(const Vector3& Pos, ViewProjection* viewProjection){
	input = Input::GetInstance();
	
	playerWorldTrans.Initialize();
	model_.reset(Model::CreateFromOBJ("3Jam_jiki_model", true));
	bulletModel_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));
	//fbx
	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("3JamJiki", true));
	fbxObj3d_ = FBXObject3d::Create();
	fbxObj3d_->SetModel(fbxModel_.get());
	fbxObj3d_->PlayAnimation(0);
	

	// コリジョンマネージャに追加
	Radius = 1.0f;
	PlayerCollider = new SphereCollider(Vector4(0, Radius, 0, 0), Radius);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider);
	PlayerCollider->SetAttribute(COLLISION_ATTR_ALLIES);
	PlayerCollider->Update(playerWorldTrans.matWorld_);
	coliisionHeight = 1.0f;
	onGround = false;

	TransitionTo(new PlayerNormal);
	hp.Initialize();
	healAmount = 500;
	healNum = 3;
	isDead = false;

	energy.Initialize(1000);
	energyRecoveryAmount = 4;

	straightSpeed = 0.6f;
	diagonalSpeed = 0.7f;
	isBoost = false;
	QuickBoostCost = 200;
	boostCost = 2;
	boostTimer = 0;
	boostChangeTime = 15;

	isJump = false;
	jumpSpeed = 0.6f;
	jumpTimer = 0;
	jumpLimit = 20;

	ascendSpeed = 0.3f;
	ascendCost = 5;

	fallSpeed = -0.2f;
}

void Player::Update(){
	playerOldPos = playerWorldTrans.translation_;
	energy.Update(energyRecoveryAmount);
	PlayerRot();
	if (state_->CanMove()) {
		Move();
		Jump();
	}

	//弾
	if (input->TriggerKey(DIK_R)) {
		PlayerShooting::Reload();
	}
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {return bullet->IsDead(); });
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

	//test
	fbxObj3d_->SetPosition(Vector3(0,0,0));
	fbxObj3d_->SetRotate(Vector3(0, 0, 0));
	fbxObj3d_->SetScale(Vector3(1, 1, 1));
	//test

	PlayerSwordAttack::StaticUpdate();
	PlayerShooting::StaticUpdate();
	state_->Update(this, &playerWorldTrans);
	CheckPlayerCollider();
	WorldTransUpdate();
	fbxObj3d_->Update();

	PlayerAnimation();

	//HP
	if (input->TriggerKey(DIK_C)) {
		if (healNum > 0) {
			hp.Heal(healAmount);
		}
	}
	if (hp.IsLive() == false) {
		isDead = true;
	}



	ImGui::Begin("Player");
	//ImGui::Text("pos:%f", playerWorldTrans.translation_.x);
	//ImGui::Text("pos:%f", playerWorldTrans.translation_.y);
	//ImGui::Text("pos:%f", playerWorldTrans.translation_.z);
	//ImGui::Text("hp:%d", hp.GetHp());
	ImGui::Text("pos:%f", PlayerCollider->GetWorldPos().m[3][0]);
	ImGui::Text("pos:%f", PlayerCollider->GetWorldPos().m[3][1]);
	ImGui::Text("pos:%f", PlayerCollider->GetWorldPos().m[3][2]);
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection){
	/*model_->Draw(playerWorldTrans, viewProjection_);*/
	fbxObj3d_->Draw(playerWorldTrans, viewProjection);
	//for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
	//	bullet->Draw(viewProjection_);
	//}
}

//状態変更
void Player::TransitionTo(PlayerState* state) {
	//削除
	delete state_;
	//新規作成
	state_ = state;
	state_->Initialize();
}

void Player::DrawSprite(){
	energy.DrawSprite();
}

void Player::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	
}

void Player::ParticleDraw(ViewProjection& viewProjection_)
{
	
}

void Player::CopyParticle()
{
	
}

void Player::Move() {
	Vector3 playerMoveMent = { 0.0f,0.0f,0.0f };
	float boost = 1.0f;

	//ブースト開始
	if (input->PushKey(DIK_LSHIFT)) {
		if(input->PushKey(DIK_W)|| input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D))
			if (isBoost == false) {
				if (energy.Use(QuickBoostCost)) {
					isBoost = true;
					boostTimer = 0;
				}
			}
	}

	//ブースト時
	if (isBoost) {
		boostTimer++;
		if (boostTimer < boostChangeTime) {
			boost = 4.0f;
		}else {
			if (energy.Use(boostCost)) {
				boost = 2.0f;
			}else {
				isBoost = false;
			}
			//ブースト終了
			if (input->PushKey(DIK_LSHIFT) == false) {
				isBoost = false;
			}
		}
	}

	//通常移動
	if (input->PushKey(DIK_W)) {
		playerMoveMent += playerWorldTrans.LookVelocity.look * straightSpeed * boost;
	}
	if (input->PushKey(DIK_S)) {
		playerMoveMent += playerWorldTrans.LookVelocity.lookBack * straightSpeed * boost;
	}
	if (input->PushKey(DIK_A)) {
		playerMoveMent += playerWorldTrans.LookVelocity.lookLeft * straightSpeed * boost;
	}
	if (input->PushKey(DIK_D)) {
		playerMoveMent += playerWorldTrans.LookVelocity.lookRight * straightSpeed * boost;
	}

	//斜め移動
	if (input->PushKey(DIK_W) == 1 && input->PushKey(DIK_A) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.look_lookLeft * diagonalSpeed * boost;
	}
	if (input->PushKey(DIK_W) == 1 && input->PushKey(DIK_D) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.look_lookRight * diagonalSpeed * boost;
	}
	if (input->PushKey(DIK_S) == 1 && input->PushKey(DIK_A) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookLeft * diagonalSpeed * boost;
	}
	if (input->PushKey(DIK_S) == 1 && input->PushKey(DIK_D) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookRight * diagonalSpeed * boost;
	}

	playerWorldTrans.translation_ += playerMoveMent;
}

void Player::Jump() {
	Vector3 playerMoveMent = { 0.0f,0.0f,0.0f };
	//ジャンプ開始
	if (input->TriggerKey(DIK_SPACE)&&onGround) {
		jumpTimer = 0;
		isJump = true;	
	}

	//ジャンプ処理
	if (isJump) {
		jumpTimer++;
		if (jumpTimer < jumpLimit) {
			playerMoveMent = playerWorldTrans.LookVelocity.lookUp * jumpSpeed;
		}else {
			isJump = false;
		}
	}else if (input->PushKey(DIK_SPACE) && energy.Use(ascendCost)) {
			playerMoveMent = playerWorldTrans.LookVelocity.lookUp * ascendSpeed;
	}else{
		Fall();
	}

	playerWorldTrans.translation_ += playerMoveMent;
}

void Player::Fall() {
	// 移動
	playerWorldTrans.translation_.y += fallSpeed;
}

void Player::PlayerRot(){
	playerWorldTrans.SetRot(Vector3(0.0f, cameraRot.x, 0.0f));
}

void Player::WorldTransUpdate(){
	if (playerWorldTrans.translation_.y < -15) {
		playerWorldTrans.translation_ = { 0,0,0 };
	}
	playerWorldTrans.TransferMatrix();
}

void Player::CreatBullet(Vector3 pos, Vector3 velocity) {
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>(bulletModel_.get());
	newBullet->Initialize(pos, velocity);
	bullets.push_back(std::move(newBullet));
}

void Player::CheckPlayerCollider(){
	PlayerCollider->Update(playerWorldTrans.matWorld_);
	Vector3 moveMent = playerWorldTrans.translation_ - playerOldPos;
	moveMent = { abs(moveMent.x),abs(moveMent.y) ,abs(moveMent.z) };

	//地面メッシュコライダー
	{
		// 球の上端から球の下端までのレイキャスト
		Ray Groundray;
		Groundray.start = MyMath::Vec3ToVec4(playerOldPos);
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
				playerWorldTrans.translation_.y -= raycastHit.distance - (Radius + moveMent.y);
				playerOldPos.y = playerWorldTrans.translation_.y;
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
 				playerWorldTrans.translation_.y -= raycastHit.distance - (Radius + moveMent.y);
				playerOldPos.y = playerWorldTrans.translation_.y;
			}
		}
	}

	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 0,0,1,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.z)) {
			playerWorldTrans.translation_.z -= (moveMent.z - wallRaycastHit.distance) + Radius;
			playerOldPos.z = playerWorldTrans.translation_.z;
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 0,0,-1,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.z)) {
			playerWorldTrans.translation_.z += (moveMent.z - wallRaycastHit.distance) + Radius;
			playerOldPos.z = playerWorldTrans.translation_.z;
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.x)) {
			playerWorldTrans.translation_.x -= (moveMent.x - wallRaycastHit.distance) + Radius;
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { -1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.x)) {
			playerWorldTrans.translation_.x += (moveMent.x - wallRaycastHit.distance) + Radius;
		}
	}
}

void Player::PlayerAnimation()
{
	
	nowAnmFCount_++;
	if (input->TriggerKey(DIK_LSHIFT) && isBoost == true) {
		nowAnmFCount_ = 0;	//kaunntorisetto

		nowAnmNum_ = 3;	//何も押していない場合
		if (input->TriggerKey(DIK_W)) {
			
			nowAnmNum_ = 3;
		}
		else if (input->TriggerKey(DIK_S)) {
			
			nowAnmNum_ = 0;
		}
		else if (input->TriggerKey(DIK_A)) {
			
			nowAnmNum_ = 1;
		}
		else if (input->TriggerKey(DIK_D)) {
			
			nowAnmNum_ = 2;
		}

		if (nowAnmNum_ != oldAnmNum_) {
			fbxObj3d_->PlayAnimation(nowAnmNum_);
		}
	}
	
	if (nowAnmNum_ == 3) {	//W
		int maxFcount = 30;
		fbxObj3d_->AnimFlameInter(nowAnmFCount_, maxFcount);

		if (nowAnmFCount_ >= maxFcount) {
			nowAnmNum_ = 0;
			fbxObj3d_->PlayAnimation(nowAnmNum_);
		}

	}
	else if (nowAnmNum_ == 3) {	//W
		int maxFcount = 30;
		fbxObj3d_->AnimFlameInter(nowAnmFCount_, maxFcount);

		if (nowAnmFCount_ >= maxFcount) {
			nowAnmNum_ = 0;
			fbxObj3d_->PlayAnimation(nowAnmNum_);
		}

	}
	else {
		int maxFcount = 60;
		fbxObj3d_->AnimFlameInter(nowAnmFCount_, maxFcount);
		

		if (nowAnmFCount_ >= maxFcount) {
			nowAnmNum_ = 0;
			fbxObj3d_->PlayAnimation(nowAnmNum_);
		}
	}
	

	oldAnmNum_ = nowAnmNum_;
}
