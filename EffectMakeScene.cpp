#include "EffectMakeScene.h"
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

MakeEffectScene::MakeEffectScene() {}
MakeEffectScene::~MakeEffectScene() {

}

void MakeEffectScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	int a = 1000000;

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(), MyMath::GetAngle(180.0f), Vector3(0,0,0));
	gameCamera->SetFreeCamera(false);
	gameCamera->SetCameraMode(false);

	ParticleMan = std::make_unique<Hibana>();
	ParticleMan->Initialize(a);
	ParticleMan->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	model.reset(Model::CreateFromOBJ("ken", true));
	model1.reset(Model::CreateFromOBJ("Ground", true));

	ParticleMan->SetMeshPos(model->GetMeshData());

	ground = std::make_unique<Ground>(model1.get());
	ground->Initialze();
}

void MakeEffectScene::Update() {

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

	if (input_->PushKey(DIK_W)) {
		CameraPos += {0, 0, 0.5f};
	}
	if (input_->PushKey(DIK_S)) {
		CameraPos -= {0, 0, 0.5f};
	}
	if (input_->PushKey(DIK_D)) {
		CameraPos += {0.5f, 0, 0};
	}
	if (input_->PushKey(DIK_A)) {
		CameraPos -= {0.5f, 0, 0};
	}


	gameCamera->SetPlayerPosition(CameraPos);
	gameCamera->Update();

}

void MakeEffectScene::PostEffectDraw()
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
	Explosion::PreDraw(commandList);
	
	Explosion::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();

	Hibana::PreDraw(commandList);
	ParticleMan->Draw(*viewProjection_.get());
	Hibana::PostDraw();

	PostEffect::PostDrawScene();
}

void MakeEffectScene::CSUpdate()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	ParticleMan->CSUpdate(commandList,Vector4(0,0,0,0));

}

void MakeEffectScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

#pragma region 3Dオブジェクト描画
	ParticleCS::PreDraw(commandList);



	ParticleCS::PostDraw();

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	ground->Draw(*viewProjection_);
	//3Dオブジェクト描画後処理
	Model::PostDraw();


	ParticleHandHanabi::PreDraw(commandList);
	
	ParticleHandHanabi::PostDraw();

	Explosion::PreDraw(commandList);
	//ParticleMan->Draw(*viewProjection_.get());
	Explosion::PostDraw();

	Hibana::PreDraw(commandList);
	
	Hibana::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void MakeEffectScene::Finalize()
{
}

void MakeEffectScene::CopyData()
{

}
