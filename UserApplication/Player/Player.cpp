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
	model_.reset(Model::CreateFromOBJ("sphere", true));
	
	TransitionTo(new PlayerNormal);

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

	PlayerSwordAttack::StaticUpdate();
	PlayerShooting::StaticUpdate();
	state_->Update(this, &playerWorldTrans);
	WorldTransUpdate();

	ImGui::Begin("Player");
	ImGui::Text("pos:%f", playerWorldTrans.translation_.x);
	ImGui::Text("pos:%f", playerWorldTrans.translation_.y);
	ImGui::Text("pos:%f", playerWorldTrans.translation_.z);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection_){
	model_->Draw(playerWorldTrans, viewProjection_);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw(viewProjection_);
	}
}

//状態変更
void Player::TransitionTo(PlayerState* state) {
	//削除
	delete state_;
	//新規作成
	state_ = state;
	state_->Initialize();
}

void Player::DrawSprite(ViewProjection& viewProjection_)
{

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
	if (input->TriggerKey(DIK_LSHIFT)) {
		if(energy.Use(QuickBoostCost)){
			isBoost = true;
			boostTimer = 0;
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
	if (input->TriggerKey(DIK_SPACE)) {
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
	}else if (input->PushKey(DIK_SPACE)&& energy.Use(ascendCost)) {
			playerMoveMent = playerWorldTrans.LookVelocity.lookUp * ascendSpeed;
	}else{
		Fall();
	}

	playerWorldTrans.translation_ += playerMoveMent;
}

void Player::Fall() {
	if (playerWorldTrans.translation_.y >= 0) {
		// 移動
		playerWorldTrans.translation_.y += fallSpeed;
	}
}

void Player::PlayerRot(){
	playerWorldTrans.SetRot(Vector3(0.0f, cameraRot.x, 0.0f));
}

void Player::WorldTransUpdate()
{
	playerWorldTrans.TransferMatrix();
}

void Player::CreatBullet(Vector3 pos, Vector3 velocity) {
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(pos, velocity);
	bullets.push_back(std::move(newBullet));
}

void Player::CheckPlayerCollider()
{

}