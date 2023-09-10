#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include <Sprite.h>
#include "SplinePosition.h"

#include "PlayerBullet.h"
#include "PlayerState.h"
#include "PlayerEnergy.h"

class Player {

public://��{�֐�
	Player();
	~Player();

	void Initialize(const Vector3& Pos, ViewProjection* viewProjection);
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite();

	//��Ԉڍs
	void TransitionTo(PlayerState* state);
	//�G�l���M�[
	bool UseEnergy(int cost) { return energy.Use(cost); };

	//�p�[�e�B�N�����o���p
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

	//�e����
	void CreatBullet(Vector3 pos, Vector3 velocity);
private:
	//�v���[���[�̈ړ�
	void Move();
	//�v���C���[�̃W�����v
	void Jump();
	//����
	void Fall();
	//�v���C���[�̉�]
	void PlayerRot();
	//�v���[���[�̈ړ��̒l�X�V
	void WorldTransUpdate();
	// �v���C���[�̓����蔻��
	void CheckPlayerCollider();

public://Setter
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }
	
public://Getter
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }

private://�N���X�֘A
	Input* input = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	BaseCollider* PlayerCollider = nullptr;
	PlayerState* state_;
	PlayerEnergy energy;
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	// �R���C�_�[
private://�C�[�i���N���X

private://�ʃN���X����l�����炤
	Vector2 cameraRot;

private://�v���C���[�N���X�ϐ�
	float Radius;
	bool onGround;
	//�ړ�
	float straightSpeed;
	float diagonalSpeed;
	bool isBoost;
	int QuickBoostCost;
	int boostCost;
	int boostTimer;
	int boostChangeTime;

	//�W�����v
	bool isJump;
	float jumpSpeed;
	int jumpTimer;
	int jumpLimit;
	float ascendSpeed;
	int ascendCost;

	//����
	float fallSpeed;

	//�G�l���M�[
	int energyRecoveryAmount;
};