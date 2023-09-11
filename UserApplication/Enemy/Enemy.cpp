#include "Enemy.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
//#include <FbxLoader.h>
#include <SphereCollider.h>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(const Vector3& Pos, ViewProjection* viewProjection) {
	input = Input::GetInstance();

	enemyWorldTrans.Initialize();
	enemyWorldTrans.translation_ = Pos;
	model_.reset(Model::CreateFromOBJ("sphere", true));

	// �R���W�����}�l�[�W���ɒǉ�
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

void Enemy::Update() {
	enemyOldPos = enemyWorldTrans.translation_;
	Move();
	Jump();
	Fall();

	//�e
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->IsDead(); });
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}

	CheckEnemyCollider();
	WorldTransUpdate();

	if (hp.IsLive() == false) {
		isDead = true;
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

void Enemy::Move() {
	

}

void Enemy::Jump() {

}

void Enemy::Fall() {
	// �ړ�
	enemyWorldTrans.translation_.y += fallSpeed;
}

void Enemy::WorldTransUpdate() {
	if (enemyWorldTrans.translation_.y < -15) {
		enemyWorldTrans.translation_ = { 0,0,0 };
	}
	enemyWorldTrans.TransferMatrix();
}

void Enemy::CreatBullet(Vector3 pos, Vector3 velocity) {
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(pos, velocity);
	bullets.push_back(std::move(newBullet));
}

void Enemy::CheckEnemyCollider() {
	enemyCollider->Update(enemyWorldTrans.matWorld_);
	Vector3 moveMent = enemyWorldTrans.translation_ - enemyOldPos;
	moveMent = { abs(moveMent.x),abs(moveMent.y) ,abs(moveMent.z) };

	//�n�ʃ��b�V���R���C�_�[
	{
		// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
		Ray Groundray;
		Groundray.start = MyMath::Vec3ToVec4(enemyOldPos);
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
				enemyWorldTrans.translation_.y -= raycastHit.distance - (Radius + moveMent.y);
				enemyOldPos.y = enemyWorldTrans.translation_.y;
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
				enemyWorldTrans.translation_.y -= raycastHit.distance - (Radius + moveMent.y);
				enemyOldPos.y = enemyWorldTrans.translation_.y;
			}
		}
	}

	{
		//�����b�V���R���C�_�[
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(enemyOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 0,0,1,0 };
		RaycastHit wallRaycastHit;
		// �X���[�Y�ɍ������ׂ̋z������

		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.z)) {
			enemyWorldTrans.translation_.z -= (moveMent.z - wallRaycastHit.distance) + Radius;
			enemyOldPos.z = enemyWorldTrans.translation_.z;
		}

	}
	{
		//�����b�V���R���C�_�[
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(enemyOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 0,0,-1,0 };
		RaycastHit wallRaycastHit;
		// �X���[�Y�ɍ������ׂ̋z������

		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.z)) {
			enemyWorldTrans.translation_.z += (moveMent.z - wallRaycastHit.distance) + Radius;
			enemyOldPos.z = enemyWorldTrans.translation_.z;
		}
	}
	{
		//�����b�V���R���C�_�[
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(enemyOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { 1,0,0,0 };
		RaycastHit wallRaycastHit;
		// �X���[�Y�ɍ������ׂ̋z������

		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.x)) {
			enemyWorldTrans.translation_.x -= (moveMent.x - wallRaycastHit.distance) + Radius;
		}

	}
	{
		//�����b�V���R���C�_�[
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(enemyOldPos);
		wall.start.y += coliisionHeight;
		wall.dir = { -1,0,0,0 };
		RaycastHit wallRaycastHit;
		// �X���[�Y�ɍ������ׂ̋z������

		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius + moveMent.x)) {
			enemyWorldTrans.translation_.x += (moveMent.x - wallRaycastHit.distance) + Radius;
		}
	}
}