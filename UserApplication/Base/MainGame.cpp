#include "MainGame.h"
#include "SceneFactory.h"


void MainGame::Initialize()
{
	// ���N���X�̏���������
	Framework::Initialize();

	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
	SceneManager::GetInstance()->ChangeScene("EffectMakeScene");
}

void MainGame::Finalize()
{
	// ���N���X�̏I������
	Framework::Finalize();
}

void MainGame::Update()
{
	// ���N���X�̍X�V����
	Framework::Update();
}

void MainGame::PostEffectDraw()
{
	sceneManager_->PostEffectDraw();
}

void MainGame::Draw()
{
	// �Q�[���V�[���̕`��
	sceneManager_->Draw();
}

void MainGame::CopyData()
{
	sceneManager_->CopyData();
}

void MainGame::CSUpdate()
{
	sceneManager_->CSUpdate();
}