﻿#pragma once

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
#include "EnemyManager.h"
#include "Tower.h"
#include "LoadLevelEditor.h"

#include "TouchableObject.h"
#include <Skydome.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

public: // メンバ関数

	GameScene();
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	// 終了処理
	void Finalize() override;
	//コピー処理
	void CopyData() override;
	//ポストエフェクトを掛けるやつ
	void PostEffectDraw() override;
	//CSアップデート
	void CSUpdate() override;

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_ = nullptr;

	int shadeNumber = 3;
	int range = 0;//ぼかし強度
	int samples = 5;

	uint32_t MaxFream = 60;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;

	int gameTimer;
	int gameLimit;

	Vector2 center = {0.5f,0.5f};

	CollisionManager* collisionManager = nullptr;//当たり判定
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<GameCamera> gameCamera;
	std::unique_ptr<Player> player_;
	std::unique_ptr<EnemyManager> enemyManager_;
	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Ground> ground;
	std::unique_ptr<Model> towerModel_;
	std::unique_ptr<Tower> tower;
};