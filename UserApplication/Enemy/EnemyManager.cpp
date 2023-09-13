#include"EnemyManager.h"
#include "External/json.hpp"
#include <fstream>
#include <cassert>
#include <stdio.h>

const std::string EnemyManager::kDefaultBaseDirectory = "Resources/levels/";
const std::string EnemyManager::kExtension = ".json";

EnemyManager::~EnemyManager() {
}

void EnemyManager::Initialize(const std::string& fileName) {
	levelData = LoadFile(fileName);
	model_.reset(Model::CreateFromOBJ("sphere", true));
	bulletModel_.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));
}

bool EnemyManager::IsAllEnemyDead() {
	if (levelData->enemys.back().spawnTimer < 0) {
		if (enemys.back()->IsDead()) {
			return true;
		}
	}

	return false;
}

void EnemyManager::Update() {
	CreateEnemy();

	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Update();
	}
}

void EnemyManager::Draw(ViewProjection& viewProjection_) {
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Draw(viewProjection_);
	}
}

void EnemyManager::CreateEnemy() {
	// レベルデータからオブジェクトを生成、配置
	for (auto& enemyData : levelData->enemys) {
		if (enemyData.spawnTimer == 0) {
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(enemyData.translation, model_.get(), bulletModel_.get());
			enemys.push_back(std::move(newEnemy));
		}
		enemyData.spawnTimer--;
	}
}

EnemyManager::LevelData* EnemyManager::LoadFile(const std::string& fileName) {
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));
		// 種別を取得
		std::string type = object["type"].get<std::string>();
		// MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->enemys.emplace_back(LevelData::EnemyData{});
			// 今追加した要素の参照を得る
			LevelData::EnemyData& enemyData = levelData->enemys.back();

			if (object.contains("file_name")) {
				// ファイル名
				enemyData.fileName = object["file_name"];
			}

			if (object.contains("spawn_time")) {
				std::string time = object["spawn_time"];
				enemyData.spawnTimer = stoi(time);
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			enemyData.translation.x = static_cast<float>(transform["translation"][0]);
			enemyData.translation.y = static_cast<float>(transform["translation"][2]);
			enemyData.translation.z = static_cast<float>(transform["translation"][1]);
			// 回転角
			enemyData.rotation.x = -static_cast<float>(transform["rotation"][1]);
			enemyData.rotation.y = -static_cast<float>(transform["rotation"][2]);
			enemyData.rotation.z = static_cast<float>(transform["rotation"][0]);

			// スケーリング
			enemyData.scaling.x = static_cast<float>(transform["scaling"][1]);
			enemyData.scaling.y = static_cast<float>(transform["scaling"][2]);
			enemyData.scaling.z = static_cast<float>(transform["scaling"][0]);
		}
		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
	}
	return levelData;
}