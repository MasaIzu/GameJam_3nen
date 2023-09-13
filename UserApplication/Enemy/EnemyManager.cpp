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
	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
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
	// �A�����ăt���p�X�𓾂�
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// �t�@�C���X�g���[��
	std::ifstream file;

	// �t�@�C�����J��
	file.open(fullpath);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	// JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	// ��
	file >> deserialized;

	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	// ���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* levelData = new LevelData();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));
		// ��ʂ��擾
		std::string type = object["type"].get<std::string>();
		// MESH
		if (type.compare("MESH") == 0) {
			// �v�f�ǉ�
			levelData->enemys.emplace_back(LevelData::EnemyData{});
			// ���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::EnemyData& enemyData = levelData->enemys.back();

			if (object.contains("file_name")) {
				// �t�@�C����
				enemyData.fileName = object["file_name"];
			}

			if (object.contains("spawn_time")) {
				std::string time = object["spawn_time"];
				enemyData.spawnTimer = stoi(time);
			}

			// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			// ���s�ړ�
			enemyData.translation.x = static_cast<float>(transform["translation"][0]);
			enemyData.translation.y = static_cast<float>(transform["translation"][2]);
			enemyData.translation.z = static_cast<float>(transform["translation"][1]);
			// ��]�p
			enemyData.rotation.x = -static_cast<float>(transform["rotation"][1]);
			enemyData.rotation.y = -static_cast<float>(transform["rotation"][2]);
			enemyData.rotation.z = static_cast<float>(transform["rotation"][0]);

			// �X�P�[�����O
			enemyData.scaling.x = static_cast<float>(transform["scaling"][1]);
			enemyData.scaling.y = static_cast<float>(transform["scaling"][2]);
			enemyData.scaling.z = static_cast<float>(transform["scaling"][0]);
		}
		// TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
		if (object.contains("children")) {

		}
	}
	return levelData;
}