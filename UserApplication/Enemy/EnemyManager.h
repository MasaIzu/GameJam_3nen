#pragma once
#include "Enemy.h"
#include <memory>
#include"Model.h"
#include<map>

class EnemyManager {
private:
	struct LevelData {
		struct EnemyData {
			// ファイル名
			std::string fileName;
			// 平行移動
			Vector3 translation;
			// 回転角
			Vector3 rotation;
			// スケーリング
			Vector3 scaling;
			//生成までの時間
			int spawnTimer;
		};
		std::vector<EnemyData> enemys;
	};

public:
	~EnemyManager();
	void Initialize(const std::string& fileName);
	void Update();
	void Draw(ViewProjection& viewProjection_);

private:
	LevelData* LoadFile(const std::string& fileName);
	void CreateEnemy();

private:
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

	LevelData* levelData;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> bulletModel_;

	std::list<std::unique_ptr<Enemy>> enemys;
};