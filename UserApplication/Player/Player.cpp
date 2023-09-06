#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
//#include <FbxLoader.h>
#include <SphereCollider.h>

#include"PlayerNormal.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(const Vector3& Pos, ViewProjection* viewProjection)
{
	input = Input::GetInstance();
	playerWorldTrans.Initialize();
	model_.reset(Model::CreateFromOBJ("sphere", true));
	TransitionTo(new PlayerNormal);

	straightSpeed = 0.6f;
	diagonalSpeed = 0.7f;
}

void Player::Update()
{
	Move();
	state_->Update(this, &playerWorldTrans);
	WorldTransUpdate();
}

void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(playerWorldTrans, viewProjection_);
}

//ó‘Ô•ÏX
void Player::TransitionTo(PlayerState* state) {
	//íœ
	delete state_;
	//V‹Kì¬
	state_ = state;
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

	if (input->PushKey(DIK_LSHIFT)) {
		boost = 2.0f;
	}

	//’ÊíˆÚ“®
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

	//ŽÎ‚ßˆÚ“®
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

void Player::WorldTransUpdate()
{
	playerWorldTrans.TransferMatrix();
}

void Player::CheckPlayerCollider()
{

}