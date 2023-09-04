#include "SceneManager.h"
#include <cassert>

SceneManager* SceneManager::SceneManager_ = nullptr;

void SceneManager::Update()
{
	// ���̃V�[���̗\�񂪂���Ȃ�
	if (nextScene_)
	{
		// ���V�[���̏I��
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}

		// �V�[���؂�ւ�
		scene_ = nextScene_;
		nextScene_ = nullptr;

		// �V�[���}�l�[�W�����Z�b�g
		scene_->SetSceneManager(this);

		// ���̃V�[��������������
		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::PostEffectDraw()
{
	scene_->PostEffectDraw();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::Finalize()
{
	// �Ō�̃V�[���̏I���Ɖ��
	scene_->Finalize();
	delete scene_;
	delete SceneManager_;
}

void SceneManager::CopyData()
{
	scene_->CopyData();
}

void SceneManager::CSUpdate()
{
	scene_->CSUpdate();
}

void SceneManager::SetSceneFactory(AbstractSceneFactory* sceneFactory)
{
	sceneFactory_ = sceneFactory;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	this->nextScene_ = nextScene_;

	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// ���V�[���𐶐�
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}

SceneManager* SceneManager::GetInstance()
{
	if (SceneManager_ == nullptr)
	{
		SceneManager_ = new SceneManager();
	}

	return SceneManager_;
}