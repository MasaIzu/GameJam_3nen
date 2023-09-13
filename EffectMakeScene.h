#pragma once

#include "DirectXCore.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin.h"
#include "ParticleCS.h"

#include "BaseScene.h"
#include "SceneManager.h"

#include<memory>
#include<vector>
#include "GameCamera.h"
#include <CollisionManager.h>

#include "Ground.h"

#include "Player.h"
#include "LoadLevelEditor.h"

#include "TouchableObject.h"
#include <Skydome.h>
#include <ParticleHandHanabi.h>
#include <Explosion.h>
#include <Hibana.h>

/// <summary>
/// �Q�[���V�[��
/// </summary>
class MakeEffectScene : public BaseScene {

public: // �����o�֐�

	MakeEffectScene();
	~MakeEffectScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
	// �I������
	void Finalize() override;
	//�R�s�[����
	void CopyData() override;
	//�|�X�g�G�t�F�N�g���|������
	void PostEffectDraw() override;
	//CS�A�b�v�f�[�g
	void CSUpdate() override;

private: // �����o�ϐ�
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;


	std::unique_ptr<ViewProjection> viewProjection_;//�r���[�v���W�F�N�V����
	std::unique_ptr<Hibana> ParticleMan;
	std::unique_ptr<GameCamera> gameCamera;
	std::unique_ptr<Ground> ground;
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> model1;

	int shadeNumber = 3;
	int range = 0;//�ڂ������x
	int samples = 5;

	uint32_t MaxFream = 60;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;


	Vector2 center = { 0.5f,0.5f };

	Vector3 CameraPos;

};