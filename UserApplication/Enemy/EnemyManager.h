#pragma once
#include "Enemy.h"
#include <memory>
#include"Model.h"
#include<map>

class EnemyManager {
private:
	struct LevelData {
		struct EnemyData {
			// �t�@�C����
			std::string fileName;
			// ���s�ړ�
			Vector3 translation;
			// ��]�p
			Vector3 rotation;
			// �X�P�[�����O
			Vector3 scaling;
			//�����܂ł̎���
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
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;
	// �t�@�C���g���q
	static const std::string kExtension;

	LevelData* levelData;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> bulletModel_;

	std::list<std::unique_ptr<Enemy>> enemys;
};