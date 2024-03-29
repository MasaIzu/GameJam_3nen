#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

class SceneManager final
{
public: // メンバ関数

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ポストエフェクをかけるオブジェクトの描画
	/// </summary>
	void PostEffectDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// コピー処理
	/// </summary>
	void CopyData();

	//CSアップデート
	void CSUpdate();

	// 次のシーン予約
	void SetSceneFactory(AbstractSceneFactory* sceneFactory);
	void ChangeScene(const std::string& sceneName);

	static SceneManager* GetInstance();

private:
	static SceneManager* SceneManager_;

private:
	// 今のシーン（実行中シーン）
	BaseScene* scene_ = nullptr;
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(const SceneManager&) = delete;
};
