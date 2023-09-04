#pragma once

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
public:
	//�R���X�g���N�^
	Vector4();								//��x�N�g���Ƃ���
	Vector4(float x, float y, float z, float w);		//x����,y����,z���� ���w�肵�Đ���

	//�����o�֐�
	float length() const;					//�m����(����)�����߂�
	Vector4& normalize();					//���K������
	float dot(const Vector4& v) const;		//���ς����߂�
	Vector4 cross(const Vector4& v) const;	//�O�ς����߂�


	float Vector3Length()const;//�m���������߂�
	float Vector3LengthSquared()const;
	Vector4 Vector3Normalization()const;//���K������
	Vector4& Vector3Normal();//���K������
	float Vector3Dot(const Vector4& v)const;//���ς����߂�
	Vector4 Vector3Cross(const Vector4& v)const;//�O�ς����߂�


	//�P�����Z�q�I�[�o�[���[�h
	Vector4 operator+() const;
	Vector4 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);

};


//�@2�����Z�q�I�[�o�[���[�h
//�@���낢��Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);

void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest);

Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t);