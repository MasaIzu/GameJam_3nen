#pragma once

#include <string>
#include <vector>
#include "Vector3.h"
#include <Model.h>
#include "WorldTransform.h"
#include <map>

#include "Ground.h"
#include "TouchableObject.h"

// ���x���f�[�^
struct LevelData {

	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		Vector3 translation;
		// ��]�p
		Vector3 rotation;
		// �X�P�[�����O
		Vector3 scaling;
	};

	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;
};

struct ModelData {
	Model* model;
	WorldTransform worldTrans;
};

// ���x���f�[�^�̐���
class LoadLevelEditor {

public:// �萔
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;
	// �t�@�C���g���q
	static const std::string kExtension;

public:// �����o�֐�

	LoadLevelEditor();
	~LoadLevelEditor();

	/// <summary>
	/// ���x���f�[�^�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	LevelData* LoadFile(const std::string& fileName);

	void Initialize(const std::string& fileName);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	std::vector<Vector3> GetFirstSpline() { return FirstSplineVec; }
	std::vector<Vector3> GetSpline() { return splineVec; }
	std::vector<Vector3> GetFinalSpline() { return FinalSplineVec; }
	std::vector<Vector3> GetBossSpline() { return BossEnemySplineVec; }
	std::vector<Vector3> GetStage2SropSpline() { return stage2SropSplineVec; }
private:
	std::unique_ptr<LevelData> levelData;
	std::map<std::string, Model*> models;
	std::vector<ModelData> LoadedObjects;
	std::vector<ModelData> NewLoadObjects;
	std::list<Ground*> ground;
	std::vector<Vector3> FirstSplineVec;
	std::vector<Vector3> splineVec;
	std::vector<Vector3> FinalSplineVec;
	std::vector<Vector3> BossEnemySplineVec;
	std::vector<Vector3> stage2SropSplineVec;

	//�ŏ�����ǂݍ��ނ��
	std::unique_ptr<Model> modelSrop;
	std::unique_ptr<Model> modelGround;
	std::unique_ptr<Model> modelGoal;
	std::unique_ptr<Model> modelNormalEnemy;
	std::unique_ptr<Model> modelBulletShotEnemy;
};