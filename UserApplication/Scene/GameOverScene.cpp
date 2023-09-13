#include "GameOverScene.h"

void GameOverScene::Initialize()
{
	input = Input::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/gameover.png"));
	sceneManager_ = SceneManager::GetInstance();

}

void GameOverScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}
}

void GameOverScene::PostEffectDraw()
{
}

void GameOverScene::Draw()
{
	sprite_->Draw({ 640,360 }, { 1,1,1,1 });
}

void GameOverScene::Finalize()
{
}

void GameOverScene::CopyData()
{
}

void GameOverScene::CSUpdate()
{
}
