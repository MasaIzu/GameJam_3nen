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

void Player::Initialize(const Vector3& Pos, ViewProjection* viewProjection) {
	input = Input::GetInstance();

	playerWorldTrans.Initialize();
	swordColliderTrans.Initialize();
	model_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));
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

	straightSpeed = 2.6f;
	diagonalSpeed = 2.7f;
	isBoost = false;
	QuickBoostCost = 200;
	boostCost = 2;
	boostTimer = 0;
	boostChangeTime = 15;

	isMove = false;

	isJump = false;
	jumpSpeed = 2.6f;
	jumpTimer = 0;
	jumpLimit = 20;

	ascendSpeed = 0.7f;
	ascendCost = 5;


	fallSpeed = -0.8f;

	for (uint32_t i = 0; i < particleCount; i++) {
		Particle[i] = std::make_unique<ParticleHandHanabi>();
		int MaxParticleCount = 15000;
		Particle[i]->Initialize(MaxParticleCount);
		Particle[i]->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));
	}

}

void Player::Update() {
	isMove = false;
	playerOldPos = playerWorldTrans.translation_;
	energy.Update(energyRecoveryAmount);
	if (state_->CanMove()) {
		Move();
		Jump();
	}
	CheckPlayerCollider();
	PlayerRot();
	LockOn();
	WorldTransUpdate();

	PlayerSwordAttack::StaticUpdate();
	PlayerShooting::StaticUpdate();


	//弾
	if (input->TriggerKey(DIK_R)) {
		PlayerShooting::Reload();
	}
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {return bullet->IsDead(); });
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}


	//test
	//fbxObj3d_->SetPosition(playerWorldTrans.translation_);
	fbxObj3d_->SetRotate(Vector3(0, 0, 0));
	fbxObj3d_->SetScale(Vector3(1, 1, 1));
	//test

	PlayerSwordAttack::StaticUpdate();
	PlayerShooting::StaticUpdate();
	state_->Update(this, &playerWorldTrans, &swordColliderTrans);
	CheckPlayerCollider();
	WorldTransUpdate();

	//fbxObj3d_->wtf.translation_ = playerWorldTrans.translation_;
	//fbxObj3d_->wtf.rotation_ = playerWorldTrans.rotation_;
	//fbxObj3d_->wtf.scale_ = playerWorldTrans.scale_;
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

	uint32_t BoneNumber = 31;

	swordColliderTrans.translation_ = MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(BoneNumber) * playerWorldTrans.matWorld_);

	swordColliderTrans.TransferMatrix();

	ImGui::Begin("Player");
	ImGui::Text("pos:%f,%f,%f", playerWorldTrans.translation_.x, playerWorldTrans.translation_.y, playerWorldTrans.translation_.z);
	ImGui::Text("sowrdPos:%f,%f,%f", swordColliderTrans.translation_.x, swordColliderTrans.translation_.y, swordColliderTrans.translation_.z);
	ImGui::Text("eye:%f,%f,%f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("cameraTarget:%f,%f,%f", cameraTargetPos.x, cameraTargetPos.y, cameraTargetPos.z);
	ImGui::Text("target:%f,%f,%f", targetPos.x, targetPos.y, targetPos.z);
	ImGui::End();

	uint32_t kakatoRightBoneNumber = 7;
	uint32_t kakatoLeftBoneNumber = 12;
	uint32_t kataLeftBoneNumber = 50;
	uint32_t kataRightBoneNumber = 52;

	Vector4 kakatoLeft = MyMath::Vec3ToVec4(playerWorldTrans.LookVelocity.lookBack);
	Vector4 kakatoRight = MyMath::Vec3ToVec4(playerWorldTrans.LookVelocity.lookBack);
	Vector4 kataLeft = MyMath::Vec3ToVec4(playerWorldTrans.LookVelocity.lookBack_lookLeft);
	Vector4 kataRight = MyMath::Vec3ToVec4(playerWorldTrans.LookVelocity.lookBack_lookRight);

	Vector3 tyouseiLeft = playerWorldTrans.LookVelocity.lookBack_lookLeft + Vector3(0, 0.25f, 0);
	Vector3 tyouseiRight = playerWorldTrans.LookVelocity.lookBack_lookRight + Vector3(0, 0.25f, 0);

	Particle[0]->CSUpdate(MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(kakatoRightBoneNumber) * playerWorldTrans.matWorld_) - Vector3(0, 0.5, 0)),0,&kakatoLeft);
	Particle[1]->CSUpdate(MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(kakatoLeftBoneNumber) * playerWorldTrans.matWorld_) - Vector3(0, 0.5, 0)),0,&kakatoLeft);
	Particle[2]->CSUpdate(MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(kataLeftBoneNumber) * playerWorldTrans.matWorld_) + tyouseiRight),1, &kataRight);
	Particle[3]->CSUpdate(MyMath::Vec3ToVec4(MyMath::GetWorldTransform(fbxObj3d_->GetBonesMatPtr(kataRightBoneNumber) * playerWorldTrans.matWorld_) + tyouseiLeft),1, &kataLeft);

}


void Player::Draw(const ViewProjection& viewProjection){
	model_->Draw(swordColliderTrans, viewProjection);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw(viewProjection);
	}

}

void Player::FbxDraw(const ViewProjection& viewProjection) {
	fbxObj3d_->Draw(playerWorldTrans, viewProjection);
}

//状態変更
void Player::TransitionTo(PlayerState* state) {
	//削除
	delete state_;
	//新規作成
	state_ = state;
	state_->Initialize();
}

void Player::DrawSprite() {
	energy.DrawSprite();
}

void Player::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{

}

void Player::ParticleDraw(ViewProjection& viewProjection_)
{
	for (uint32_t i = 0; i < particleCount; i++) {
		Particle[i]->Draw(viewProjection_);
	}
}

void Player::CopyParticle()
{

}

void Player::Move() {
	Vector3 playerMoveMent = { 0.0f,0.0f,0.0f };
	float boost = 1.0f;

	//ブースト開始
	if (input->PushKey(DIK_LSHIFT)) {
		if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D))
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
		}
		else {
			if (energy.Use(boostCost)) {
				boost = 2.0f;
			}
			else {
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
		isMove = true;
		playerMoveMent += {sinf(cameraRot.x)* (straightSpeed* boost), 0, cosf(cameraRot.x)* (straightSpeed* boost)};
	}
	if (input->PushKey(DIK_S)) {
		isMove = true;
		playerMoveMent += {sinf(cameraRot.x + 3.14f)* (straightSpeed* boost), 0, cosf(cameraRot.x + 3.14f)* (straightSpeed* boost)};
	}

	if (input->PushKey(DIK_A)) {
		isMove = true;
		playerMoveMent += {sinf(cameraRot.x - 1.57f)* (straightSpeed* boost), 0, cosf(cameraRot.x - 1.57f)* (straightSpeed* boost)};
	}
	if (input->PushKey(DIK_D)) {
		isMove = true;
		playerMoveMent += {sinf(cameraRot.x + 1.57f)* (straightSpeed* boost), 0, cosf(cameraRot.x + 1.57f)* (straightSpeed* boost)};
	}

	playerWorldTrans.translation_ += playerMoveMent;
}

void Player::Jump() {
	Vector3 playerMoveMent = { 0.0f,0.0f,0.0f };
	//ジャンプ開始
	if (input->TriggerKey(DIK_SPACE) && onGround) {
		jumpTimer = 0;
		isJump = true;
	}

	//ジャンプ処理
	if (isJump) {
		jumpTimer++;
		if (jumpTimer < jumpLimit) {
			playerMoveMent = playerWorldTrans.LookVelocity.lookUp * jumpSpeed;
		}
		else {
			isJump = false;
		}
	}
	else if (input->PushKey(DIK_SPACE) && energy.Use(ascendCost)) {
		playerMoveMent = playerWorldTrans.LookVelocity.lookUp * ascendSpeed;
	}
	else {
		Fall();
	}

	playerWorldTrans.translation_ += playerMoveMent;
}

void Player::Fall() {
	// 移動
	playerWorldTrans.translation_.y += fallSpeed;
}

void Player::PlayerRot() {
	if (playerWorldTrans.translation_.x != playerOldPos.x || playerWorldTrans.translation_.z != playerOldPos.z) {
		Vector3 moveVec = playerWorldTrans.translation_ - playerOldPos;
		playerWorldTrans.SetRot(Vector3(0.0f, atan2f(moveVec.x, moveVec.z), 0.0f));
	}
}

void Player::WorldTransUpdate() {
	if (playerWorldTrans.translation_.y < -75) {
		playerWorldTrans.translation_ = { 0,0,0 };
	}
	playerWorldTrans.TransferMatrix();
	
}

void Player::CreatBullet(Vector3 pos, Vector3 velocity) {
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>(bulletModel_.get());
	newBullet->Initialize(pos, velocity);
	bullets.push_back(std::move(newBullet));
	if (PlayerState::GetIsLockOn()) {
		Vector3 moveVec = targetPos - playerWorldTrans.translation_;
		playerWorldTrans.SetRot(Vector3(0.0f, atan2f(moveVec.x, moveVec.z), 0.0f));
	}
}

void Player::CheckPlayerCollider() {
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
	nowMoveN_ = state_->GetStateName();
	nowAnmFCount_++;
	if (input->TriggerKey(DIK_LSHIFT) && isBoost == true) {
		nowAnmFCount_ = 0;	//kaunntorisetto

		nowAnmNum_ = 2;	//何も押していない場合
		if (input->TriggerKey(DIK_W)) {

			nowAnmNum_ = 2;
		}
		/*else if (input->TriggerKey(DIK_S)) {

			nowAnmNum_ = 1;
		}
		else if (input->TriggerKey(DIK_A)) {

			nowAnmNum_ = 2;
		}
		else if (input->TriggerKey(DIK_D)) {

			nowAnmNum_ = 3;
		}*/


		
	}
	//攻撃
	if (nowMoveN_ == "SWORD" && oldMoveN_ != nowMoveN_) {
		nowAnmNum_ = 5;
		nowAnmFCount_ = 0;
	}

	if (nowAnmNum_ != oldAnmNum_) {
		fbxObj3d_->PlayAnimation(nowAnmNum_);
	}

	

	//インターフェース用
	if (nowAnmNum_ == 2) {	//W
		int maxFcount = 25;
		fbxObj3d_->AnimFlameInter(nowAnmFCount_, maxFcount);

		if (nowAnmFCount_ >= maxFcount) {
			nowAnmNum_ = 7;
			fbxObj3d_->PlayAnimation(nowAnmNum_);
		}

	}
	else if (nowAnmNum_ == 3) {	//W
		int maxFcount = 30;
		fbxObj3d_->AnimFlameInter(nowAnmFCount_, maxFcount);

		if (nowAnmFCount_ >= maxFcount) {
			nowAnmNum_ = 7;
			fbxObj3d_->PlayAnimation(nowAnmNum_);
		}

	}
	else if (nowAnmNum_ == 5) {
		int maxFcount = 30;
		fbxObj3d_->AnimFlameInter(nowAnmFCount_, maxFcount);

		if (nowAnmFCount_ >= maxFcount) {
			nowAnmNum_ = 7;
			fbxObj3d_->PlayAnimation(nowAnmNum_);
		}
	}
	else {
		int maxFcount = 60;
		fbxObj3d_->AnimFlameInter(nowAnmFCount_, maxFcount);


		if (nowAnmFCount_ >= maxFcount) {
			nowAnmNum_ = 7;
			fbxObj3d_->PlayAnimation(nowAnmNum_);
		}
	}

	

	oldAnmNum_ = nowAnmNum_;
	oldMoveN_ = nowMoveN_;
}

void Player::LockOn() {
	Ray eye;
	eye.start = MyMath::Vec3ToVec4(cameraPos);
	eye.dir = MyMath::Vec3ToVec4(cameraTargetPos - cameraPos);
	eye.dir.normalize();
	RaycastHit RaycastHit;
	float distance = 500;

	if (CollisionManager::GetInstance()->Raycast(eye, COLLISION_ATTR_ENEMYS, &RaycastHit, distance)) {
		targetPos = MyMath::Vec4ToVec3(RaycastHit.inter);
		PlayerState::SetIsLockOn(true);
	}else {
		targetPos = playerWorldTrans.translation_ + playerWorldTrans.LookVelocity.look * 10;
		PlayerState::SetIsLockOn(false);
	}
	PlayerState::SettargetPos(targetPos);
}

bool Player::IsDead() {
	if (hp.IsLive()) {
		return false;
	}
	return true;
}