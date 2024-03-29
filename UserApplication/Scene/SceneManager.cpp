#include "SceneManager.h"
#include <cassert>

SceneManager* SceneManager::SceneManager_ = nullptr;

void SceneManager::Update()
{
	// 次のシーンの予約があるなら
	if (nextScene_)
	{
		// 旧シーンの終了
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}

		// シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャをセット
		scene_->SetSceneManager(this);

		// 次のシーンを初期化する
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
	// 最後のシーンの終了と解放
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

	// 次シーンを生成
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
