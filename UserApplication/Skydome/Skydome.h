#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <assert.h>
#include "ViewProjection.h"
#include "affin.h"

/// <summary>
/// 天球
/// </summary>
class Skydome {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection_);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_;

};