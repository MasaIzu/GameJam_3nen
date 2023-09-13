#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
//#include "FbxLoader.h"
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include"PostEffect.h"
#include"WinApp.h"

#include "fbxsdk.h"
#include"FBXObject3d.h"
#include"FbxModel.h"
#include"EnemyState.h"
#include "ParticleHandHanabi.h"

GameScene::GameScene() {}
GameScene::~GameScene() {

}

void GameScene::Initialize() {
	collisionManager = CollisionManager::GetInstance();
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	FbxManager* fbxManager = FbxManager::Create();
	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();
  
	FBXObject3d::SetCamera(viewProjection_.get());

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0, 0, 0), viewProjection_.get());

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(), MyMath::GetAngle(180.0f), player_->GetPlayerPos());

	towerModel_.reset(Model::CreateFromOBJ("Tower", true));
	tower = std::make_unique<Tower>(towerModel_.get());
	tower->Initialize();

	EnemyState::SetTowerPos(tower->GetTowerPos());

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize("battleField");
 
	groundModel_.reset(Model::CreateFromOBJ("battleField", true));
	ground = std::make_unique<Ground>(groundModel_.get());
	ground->Initialze();


	gameTimer = 0;
	gameLimit = 18000;

	ground->Update();

}

void GameScene::Update() {
	
	if (shadeNumber == 0) {
		ImGui::Begin("Not");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::End();
	}
	else if (shadeNumber == 1) {
		ImGui::Begin("averageBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SliderInt("range", &range, 0, 20);
		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 2) {
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SliderFloat("centerX", &center.x, 0, 1);
		ImGui::SliderFloat("centerY", &center.y, 0, 1);
		ImGui::SliderFloat("intensity", &intensity, 0, 1);
		ImGui::SliderInt("samples", &samples, 0, 20);
		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 3) {
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 4) {
		ImGui::Begin("CloseFilta");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);
		ImGui::SliderFloat("angle", &angle, 0.0f, 180.0f);
		ImGui::SliderFloat("angle2", &angle2, 0.0f, 180.0f);

		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
  
	tower->Update();

	player_->SetCameraRot(gameCamera->GetCameraAngle());
	player_->SetCameraPos(viewProjection_->eye);
	player_->SetCameraTargetPos(viewProjection_->target);
	player_->Update();

	enemyManager_->Update();

	Vector3 cameraPos = player_->GetPlayerPos();
	gameCamera->SetPlayerPosition(cameraPos);
	gameCamera->Update();
	
	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();


	////ゲームオーバー
	if (tower->IsDead()) {
		sceneManager_->ChangeScene("GAMEOVER");
	}
	if (player_->IsDead()) {
		sceneManager_->ChangeScene("GAMEOVER");
	}
	//クリア
	if (gameTimer > gameLimit) {
		sceneManager_->ChangeScene("CLEAR");
	}
	if (enemyManager_->IsAllEnemyDead()) {
		sceneManager_->ChangeScene("CLEAR");
	}

	gameTimer++;
}

void GameScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);

	Model::PreDraw(commandList);


	Model::PostDraw();

	////パーティクル
	ParticleHandHanabi::PreDraw(commandList);
	//player_->ParticleDraw(*viewProjection_.get());
	ParticleHandHanabi::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();



	PostEffect::PostDrawScene();
}

void GameScene::CSUpdate()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

#pragma region 3Dオブジェクト描画
	ParticleManager::PreDraw(commandList);

	ParticleManager::PostDraw();

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	ground->Draw(*viewProjection_.get());
	tower->Draw(*viewProjection_.get());
	player_->Draw(*viewProjection_.get());
	enemyManager_->Draw(*viewProjection_.get());


	//3Dオブジェクト描画後処理
	Model::PostDraw();


	player_->FbxDraw(*viewProjection_.get());

	////パーティクル
	ParticleHandHanabi::PreDraw(commandList);
	player_->ParticleDraw(*viewProjection_.get());
	ParticleHandHanabi::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	player_->DrawSprite();


#pragma endregion
}

void GameScene::Finalize()
{
}

void GameScene::CopyData()
{

}
