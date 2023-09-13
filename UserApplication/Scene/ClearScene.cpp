#include "ClearScene.h"

void ClearScene::Initialize()
{
	input = Input::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/clear.png"));
	sceneManager_ = SceneManager::GetInstance();

}

void ClearScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}
}

void ClearScene::PostEffectDraw()
{
}

void ClearScene::Draw()
{
	sprite_->Draw({ 640,360 }, { 1,1,1,1 });
}

void ClearScene::Finalize()
{
}

void ClearScene::CopyData()
{
}

void ClearScene::CSUpdate()
{
}
