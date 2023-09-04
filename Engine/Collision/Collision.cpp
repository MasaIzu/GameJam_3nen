#include "Collision.h"
#include <math.h>

void Collision::ClosestPtPoint2Triangle(const Vector4& point, const Triangle& triangle, Vector4* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector4 p0_p1 = triangle.p1 - triangle.p0;
	Vector4 p0_p2 = triangle.p2 - triangle.p0;
	Vector4 p0_pt = point - triangle.p0;

	float d1 = p0_p1.Vector3Dot(p0_pt);
	float d2 = p0_p2.Vector3Dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector4 p1_pt = point - triangle.p1;

	float d3 = p0_p1.Vector3Dot(p1_pt);
	float d4 = p0_p2.Vector3Dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector4 p2_pt = point - triangle.p2;

	float d5 = p0_p1.Vector3Dot(p2_pt);
	float d6 = p0_p2.Vector3Dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector4* inter, Vector4* reject)
{
	// ���S�_�̋����̂Q�� <= ���a�̘a�̂Q��@�Ȃ����
	Vector4 tmp;
	tmp = sphereA.center - sphereB.center;
	float dist = tmp.Vector3Dot(tmp);
	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;



	if (dist <= radius2)
	{
		if (inter)
		{
			// A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = Vector4Lerp(sphereA.center, sphereB.center, t);
		}
		// �����o���x�N�g�����v�Z
		if (reject)
		{
			float rejectLen = sphereA.radius + sphereB.radius - sqrtf(dist);
			tmp = sphereA.center - sphereB.center;
			*reject = tmp.Vector3Normal();
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2SphereFastSpeedVer(const Sphere& sphereA, const Sphere& sphereB, const Sphere& checkFastSphere, const uint32_t& whichSpeher)
{
	uint32_t HowLoop = static_cast<uint32_t>(checkFastSphere.Speed / (checkFastSphere.radius * 2));
	
	if (whichSpeher == 0) {
		for (uint32_t i = 0; i < HowLoop; i++) {
			// ���S�_�̋����̂Q�� <= ���a�̘a�̂Q��@�Ȃ����
			Vector4 tmp;
			Vector4 LookVec = Vector4(checkFastSphere.look.x * i, checkFastSphere.look.y * i, checkFastSphere.look.z * i, 0);
			tmp = (sphereA.center + (LookVec)) - sphereB.center;
			float dist = tmp.Vector3Dot(tmp);
			float radius2 = sphereA.radius + sphereB.radius;
			radius2 *= radius2;

			if (dist <= radius2)
			{
				return true;
			}
		}
	}
	else {
		for (uint32_t i = 0; i < HowLoop; i++) {
			// ���S�_�̋����̂Q�� <= ���a�̘a�̂Q��@�Ȃ����
			Vector4 tmp;
			Vector4 LookVec = Vector4(checkFastSphere.look.x * i, checkFastSphere.look.y * i, checkFastSphere.look.z * i, 0);
			tmp = sphereA.center - (sphereB.center + (LookVec));
			float dist = tmp.Vector3Dot(tmp);
			float radius2 = sphereA.radius + sphereB.radius;
			radius2 *= radius2;

			if (dist <= radius2)
			{
				return true;
			}
		}
	}

	return false;
}

// �����`�Ɠ_�̓����蔻��
bool Collision::CheckRectSphere(Vector3 rectPos, Vector3 rectWidthHeightDepth1, Vector3 rectWidthHeightDepth2, Vector3 sphere, float sphereRadius) {
	// �����`�̒��S�_���v�Z
	float centerX = rectPos.x;
	float centerY = rectPos.y;
	float centerZ = rectPos.z;

	if (rectWidthHeightDepth1.x > sphere.x && sphere.x > rectWidthHeightDepth2.x) {
		if (rectWidthHeightDepth1.y > sphere.y && sphere.y > rectWidthHeightDepth2.y) {
			if (rectWidthHeightDepth1.z > sphere.z && sphere.z > rectWidthHeightDepth2.z) {
				return true;
			}
		}
	}

	// �����蔻�肪�Ȃ��ꍇ�́A�������Ă��Ȃ��Ɣ��f����
	return false;
}

bool Collision::CheckSphere2Plane(Sphere& sphere, Plane& plane, Vector4* inter)
{

	float distV = sphere.center.Vector3Dot(plane.normal);

	float dist = distV - plane.distance;

	if (fabs(dist) > sphere.radius)
	{
		return false;
	}

	// �[����_���v�Z
	if (inter)
	{
		// ���ʏ�̍Đڋߓ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector4* inter, Vector4* reject)
{

	Vector4 p;

	ClosestPtPoint2Triangle(sphere.center, triangle, &p);

	Vector4 v = p - sphere.center;

	float vDot = v.Vector3Dot(v);

	if (vDot > sphere.radius * sphere.radius)
	{
		return false;
	}

	if (inter)
	{
		*inter = p;
	}

	if (reject)
	{
		float ds = sphere.center.Vector3Dot(triangle.normal);
		float dt = triangle.p0.Vector3Dot(triangle.normal);
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}

	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector4* inter)
{
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl

	float d1 = plane.normal.Vector3Dot(ray.dir);
	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon)
	{
		return false;
	}

	float d2 = plane.normal.Vector3Dot(ray.start);
	float dist = d2 - plane.distance;

	float t = dist / -d1;

	if (t < 0) return false;

	// ��������������
	if (distance)
	{
		*distance = t;
	}

	// ��_���v�Z
	if (inter)
	{
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector4* inter)
{

	Plane plane;

	Vector4 interPlane;
	plane.normal = triangle.normal;
	plane.distance = triangle.normal.Vector3Dot(triangle.p0);

	if (!CheckRay2Plane(ray, plane, distance, &interPlane))
	{
		return false;
	}

	const float epsilon = 1.0e-5f;

	Vector4 m;

	Vector4 pt_p0 = triangle.p0 - interPlane;
	Vector4 p0_p1 = triangle.p1 - triangle.p0;
	m = pt_p0.Vector3Cross(p0_p1);

	if (m.Vector3Dot(triangle.normal) < -epsilon)
	{
		return false;
	}

	// ��p1_p2�ɂ���
	Vector4 pt_p1 = triangle.p1 - interPlane;
	Vector4 p1_p2 = triangle.p2 - triangle.p1;
	m = pt_p1.Vector3Cross(p1_p2);
	// �ӂ̊O��
	if (m.Vector3Dot(triangle.normal) < -epsilon)
	{
		return false;
	}

	// ��p2_p0�ɂ���
	Vector4 pt_p2 = triangle.p2 - interPlane;
	Vector4 p2_p0 = triangle.p0 - triangle.p2;
	m = pt_p2.Vector3Cross(p2_p0);
	// �ӂ̊O��
	if (m.Vector3Dot(triangle.normal) < -epsilon)
	{
		return false;
	}
	if (inter)
	{
		*inter = interPlane;
	}

	// �����Ȃ̂ŁA�������Ă���
	return true;

}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector4* inter)
{
	Vector4 m = ray.start - sphere.center;
	float b = m.Vector3Dot(ray.dir);
	float c = m.Vector3Dot(m) - sphere.radius * sphere.radius;
	// lay�̎n�_��sphere�̊O���ɂ���(c > 0)�Alay��sphere���痣��Ă���������
	// �����Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	// ���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f)
	{
		return false;
	}

	// ���C�͋��ƌ������Ă���B
	// ��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);
	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0) t = 0.0f;

	if (distance)
	{
		*distance = t;
	}

	if (inter)
	{
		*inter = ray.start + t * ray.dir;
	}

	return true;
}