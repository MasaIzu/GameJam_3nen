#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Uint32Vector2.h"
#include <Input.h>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Easing.h"
#include <BaseCollider.h>

class GameCamera {

public://基本関数

	GameCamera(uint32_t window_width, uint32_t window_height);
	~GameCamera();

	void Initialize(ViewProjection* viewProjection_, const float& cameraAngle, const Vector3& pos);
	void InitializeCameraPosition(const float& cameraAngle);
	void Update();

public://リセット
	void MousePositionReset();
	void MousePositionReset(Vector2& vector2,bool setORnot);//少し細工する

private:
	void HowMuchMoved();
	void CheckCameraWhichWayMove();
	void PlaySceneCamera();
	void SceneCamera();
	void Collision();
	void CameraAngle(const float& x, const float& z);

	//カメラの視線切れているか
	bool CheckBetweenToCameraCollider();

public://getter
	float GetFovAngle() { return MyMath::GetAngle(Fov); }
	float GetCameraDistanse() const { return PlayerToCameraVecDistance; }
	float GetMaxDistance() const { return cameraDistance_; }
	Vector2 GetCameraAngle() const { return Vector2(mouseMoved.y - MyMath::PI, mouseMoved.x); }
	Vector3 GetCameraRotVec3() { return this->rot; }
	Vector3 GetEyeToTagetVecDistance(const float& distance) const;
	Vector3 GetPlayerDistanceEyePos(const Vector3& playerPos_);
	Matrix4 GetCameraRot() { return this->CameraRot; }

public://setter
	void SetCameraMode(const bool& mode) { cameraMode = mode; }
	void SetPlayerPosition(const Vector3& pos) { playerPos_ = pos; }
	void SetFreeCamera(const bool& mode) { FreeCamera = mode; }
	void SetCameraTargetAndPos(const Vector3& target, const Vector3& eye);

private://クラス関連
	Easing* easing_;
	Input* input_ = nullptr;
	ViewProjection* viewProjection = nullptr;

	// コライダー
	BaseCollider* CameraCollider = nullptr;

private://プレイヤークラス変数
	bool cameraMode = false;
	bool isShake = false;
	bool cameraDown = false;
	bool cameraUp = false;
	bool FreeCamera = false;

	uint32_t cameraType = 0;
	uint32_t winWidth = 0;
	uint32_t winHeight = 0;
	uint32_t cameraTime = 0;
	uint32_t MaxCameraTime = 0;
	uint32_t cameraMode_ = 0;//カメラモード
	uint32_t HowMachMovePointer = 0;
	uint32_t shakeTime = 0;
	uint32_t cameraModeChangeCountTimer = 30;

	float scaleX_ = 1.0f;// スケーリング
	float scaleY_ = 1.0f;
	float angleAroundPlayer; // プレイヤーの周りを回転する角度
	float cameraDistance_ = 30.0f;
	float angle = 0.0f;
	float cameraSpeed_ = 3;// カメラの速度
	float cameraDelay = 0.1f;// カメラが追跡する際の遅延量
	float playerCameraDelay = 0.1f;
	float playerCameraSpeed_ = 3;
	float kand = 400.0f;
	float Fov = 45.0f;
	float TargetCameraDelay = 0.05f;
	float TargetCameraSpeed_ = 1.0;
	float CameraDistanceMinus = 0.0f;
	float CameraMouseMoved = 0.0f;
	float OldMouseMoved = 0.0f;
	float CameraCollisionRadius = 0.1f;
	float CameraRayCollisionRadius = 0.2f;
	float PlayerToCameraVecDistance = 0.0f;

	Uint32Vector2 windowWH;

	Vector2 mousePos = { 0.0f,0.0f };
	Vector2 oldMousePos = { 0.0f,0.0f };
	Vector2 Mous_UP_DOWN;
	Vector2 MouseMove;
	Vector2 mouseMoved{ 0,MyMath::PI };

	Vector3 rot;
	Vector3 eye;
	Vector3 vUp;
	Vector3 playerPos_;
	Vector3 target;
	Vector3 cameraHigh = { 0,7,0 };
	Vector3 PlayerToCameraVec;
	//セットされたVec
	Vector3 SetTargetVec;
	Vector3 SetEyeVec;

	Matrix4 CameraRot;
	Matrix4 matRot;// 回転行列


};