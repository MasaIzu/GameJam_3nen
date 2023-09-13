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
	model_.reset(Model::CreateFromOBJ("3Jam_jiki_model", true));
	bulletModel_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));

	// �R���W�����}�l�[�W���ɒǉ�
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

void Player::Update() {
	playerOldPos = playerWorldTrans.translation_;
	energy.Update(energyRecoveryAmount);
	if (state_->CanMove()) {
		Move();
		Jump();
	}
	state_->Update(this, &playerWorldTrans);
	CheckPlayerCollider();
	PlayerRot();
	LockOn();
	WorldTransUpdate();

	PlayerSwordAttack::StaticUpdate();
	PlayerShooting::StaticUpdate();

	//�e
	if (input->TriggerKey(DIK_R)) {
		PlayerShooting::Reload();
	}
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {return bullet->IsDead(); });
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

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
	ImGui::Text("pos:%f,%f,%f", playerWorldTrans.translation_.x, playerWorldTrans.translation_.y, playerWorldTrans.translation_.z);
	ImGui::Text("eye:%f,%f,%f", cameraPos.x, cameraPos.y, cameraPos.z);
	ImGui::Text("cameraTarget:%f,%f,%f", cameraTargetPos.x, cameraTargetPos.y, cameraTargetPos.z);
	ImGui::Text("target:%f,%f,%f", targetPos.x, targetPos.y, targetPos.z);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection_) {
	cameraPos = viewProjection_.eye;
	cameraTargetPos = viewProjection_.target;

	model_->Draw(playerWorldTrans, viewProjection_);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw(viewProjection_);
	}
}

//��ԕύX
void Player::TransitionTo(PlayerState* state) {
	//�폜
	delete state_;
	//�V�K�쐬
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

}

void Player::CopyParticle()
{

}

void Player::Move() {
	Vector3 playerMoveMent = { 0.0f,0.0f,0.0f };
	float boost = 1.0f;

	//�u�[�X�g�J�n
	if (input->PushKey(DIK_LSHIFT)) {
		if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D))
			if (isBoost == false) {
				if (energy.Use(QuickBoostCost)) {
					isBoost = true;
					boostTimer = 0;
				}
			}
	}

	//�u�[�X�g��
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
			//�u�[�X�g�I��
			if (input->PushKey(DIK_LSHIFT) == false) {
				isBoost = false;
			}
		}
	}

	//�ʏ�ړ�
	if (input->PushKey(DIK_W)) {
		playerMoveMent += {sinf(cameraRot.x)* (straightSpeed* boost), 0, cosf(cameraRot.x)* (straightSpeed* boost)};
	}
	if (input->PushKey(DIK_S)) {
		playerMoveMent += {sinf(cameraRot.x + 3.14f)* (straightSpeed* boost), 0, cosf(cameraRot.x + 3.14f)* (straightSpeed* boost)};
	}

	if (input->PushKey(DIK_A)) {
		playerMoveMent += {sinf(cameraRot.x - 1.57f)* (straightSpeed* boost), 0, cosf(cameraRot.x - 1.57f)* (straightSpeed* boost)};
	}
	if (input->PushKey(DIK_D)) {
		playerMoveMent += {sinf(cameraRot.x + 1.57f)* (straightSpeed* boost), 0, cosf(cameraRot.x + 1.57f)* (straightSpeed* boost)};
	}

	playerWorldTrans.translation_ += playerMoveMent;
}

void Player::Jump() {
	Vector3 playerMoveMent = { 0.0f,0.0f,0.0f };
	//�W�����v�J�n
	if (input->TriggerKey(DIK_SPACE) && onGround) {
		jumpTimer = 0;
		isJump = true;
	}

	//�W�����v����
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
	// �ړ�
	playerWorldTrans.translation_.y += fallSpeed;
}

void Player::PlayerRot() {
	if (playerWorldTrans.translation_.x != playerOldPos.x || playerWorldTrans.translation_.z != playerOldPos.z) {
		Vector3 moveVec = playerWorldTrans.translation_ - playerOldPos;
		playerWorldTrans.SetRot(Vector3(0.0f, atan2f(moveVec.x, moveVec.z), 0.0f));
	}
}

void Player::WorldTransUpdate() {
	if (playerWorldTrans.translation_.y < -15) {
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

	//�n�ʃ��b�V���R���C�_�[
	{
		// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
		Ray Groundray;
		Groundray.start = MyMath::Vec3ToVec4(playerOldPos);
		Groundray.start.y += Radius;
		Groundray.dir = { 0,-1.0f,0,0 };
		RaycastHit raycastHit;

		// �ڒn���
		if (onGround) {
			// �X���[�Y�ɍ������ׂ̋z������
			const float adsDistance = 0.2f;
			// �ڒn���ێ�
			if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, Radius + adsDistance + moveMent.y)) {
				onGround = true;
				playerWorldTrans.translation_.y -= raycastHit.distance - (Radius + moveMent.y);
				playerOldPos.y = playerWorldTrans.translation_.y;
			}
			// �n�ʂ��Ȃ��̂ŗ���
			else {
				onGround = false;
			}
		}
		// �������
		else {
			if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, Radius + moveMent.y)) {
				// ���n
				onGround = true;
				playerWorldTrans.translation_.y -= raycastHit.distance - (Radius + moveMent.y);
				playerOldPos.y = playerWorldTrans.translation_.y;
			}
		}
	}

	{
		//�����b�V���R���C�_�[
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 0,0,1,0 };
		RaycastHit wallRaycastHit;
		// �X���[�Y�ɍ������ׂ̋z������

		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.z)) {
			playerWorldTrans.translation_.z -= (moveMent.z - wallRaycastHit.distance) + Radius;
			playerOldPos.z = playerWorldTrans.translation_.z;
		}

	}
	{
		//�����b�V���R���C�_�[
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 0,0,-1,0 };
		RaycastHit wallRaycastHit;
		// �X���[�Y�ɍ������ׂ̋z������

		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.z)) {
			playerWorldTrans.translation_.z += (moveMent.z - wallRaycastHit.distance) + Radius;
			playerOldPos.z = playerWorldTrans.translation_.z;
		}
	}
	{
		//�����b�V���R���C�_�[
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 1,0,0,0 };
		RaycastHit wallRaycastHit;
		// �X���[�Y�ɍ������ׂ̋z������

		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.x)) {
			playerWorldTrans.translation_.x -= (moveMent.x - wallRaycastHit.distance) + Radius;
		}

	}
	{
		//�����b�V���R���C�_�[
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { -1,0,0,0 };
		RaycastHit wallRaycastHit;
		// �X���[�Y�ɍ������ׂ̋z������

		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.x)) {
			playerWorldTrans.translation_.x += (moveMent.x - wallRaycastHit.distance) + Radius;
		}
	}
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