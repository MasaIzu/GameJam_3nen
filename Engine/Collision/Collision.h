#pragma once
#include "CollisionPrimitive.h"
#include "Vector4.h"
#include "Vector3.h"

/// <summary>
/// �����蔻��w���p�[�N���X
/// </summary>
class Collision
{
public:
	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_�i�o�͗p�j</param>
	static void ClosestPtPoint2Triangle(const Vector4& point, const Triangle& triangle, Vector4* closest);

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphereA">��A</param>
	/// <param name="sphereB">��B</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <param name="reject">�r�˃x�N�g���i�o�͗p�j</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector4* inter = nullptr, Vector4* reject = nullptr);
	static bool CheckSphere2SphereFastSpeedVer(const Sphere& sphereA, const Sphere& sphereB, const Sphere& checkFastSphere, const uint32_t& whichSpeher);

	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_�i���ʏ�̍ŋߐړ_�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Plane(Sphere& sphere, Plane& plane, Vector4* inter = nullptr);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_�i�O�p�`��̍ŋߐړ_�j</param>
	/// <param name="reject">�r�˃x�N�g���i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	//static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, AliceMathF::Vector4* inter = nullptr);
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector4* inter = nullptr, Vector4* reject = nullptr);

	static bool CheckRectSphere(Vector3 rectPos, Vector3 rectWidthHeightDepth1, Vector3 rectWidthHeightDepth2, Vector3 sphere, float sphereRadius);

	///// <summary>
	///// ���Ɩ@���t���O�p�`�̓����蔻��
	///// </summary>
	///// <param name="sphere">��</param>
	///// <param name="triangle">�O�p�`</param>
	///// <param name="inter">��_�i�O�p�`��̍ŋߐړ_�j</param>
	///// <param name="reject">�r�˃x�N�g���i�o�͗p�j</param>
	///// <returns>�������Ă��邩�ۂ�</returns>
	////static bool CheckSphere2Triangle(Sphere& sphere, Triangle& triangle, AliceMathF::Vector4* inter = nullptr, AliceMathF::Vector4* reject = nullptr);

	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="plane">����</param>
	/// <param name="inter">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector4* inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector4* inter = nullptr);

	///// <summary>
	///// ���C�Ɩ@���t���O�p�`�̓����蔻��
	///// </summary>
	///// <param name="lay">���C</param>
	///// <param name="triangle">�O�p�`</param>
	///// <param name="inter">�����i�o�͗p�j</param>
	///// <param name="inter">��_�i�o�͗p�j</param>
	///// <returns>�������Ă��邩�ۂ�</returns>
	//static bool CheckRay2Triangle(Ray& lay, Triangle& triangle, float* distance = nullptr, AliceMathF::Vector4* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="sphere">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector4* inter = nullptr);

	///// <summary>
	///// ���C�L���X�g�Ƌ��̓����蔻��
	///// </summary>
	///// <param name="lay">���C</param>
	///// <param name="sphere">��</param>
	///// <param name="sphere">�����i�o�͗p�j</param>
	///// <param name="inter">��_�i�o�͗p�j</param>
	///// <returns>�������Ă��邩�ۂ�</returns>
	//static bool CheckRayCast2Sphere(RayCast& lay, Sphere& sphere, float* distance = nullptr, AliceMathF::Vector4* inter = nullptr);

	///// <summary>
	///// OBB��OBB�̓����蔻��
	///// </summary>
	///// <param name="obb1">OBB</param>
	///// <param name="obb2">OBB</param>
	///// <returns>�������Ă��邩�ۂ�</returns>
	//static bool CheckOBB2OBB(OBB& obb1, OBB& obb2);

};