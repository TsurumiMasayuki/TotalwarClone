#pragma once
#include <DirectXMath.h>
#include <LinearMath\btVector3.h>

struct Vec3
{
public:
	explicit Vec3();
	//v��x, y, z�S�Ăɑ��
	explicit Vec3(float v);
	explicit Vec3(float x, float y);
	explicit Vec3(float x, float y, float z);
	explicit Vec3(const DirectX::XMFLOAT3& float3);
	explicit Vec3(const DirectX::XMVECTOR& vector);

	DirectX::XMVECTOR toXMVector() const;
	DirectX::XMFLOAT3 toXMFLOAT3() const;
	DirectX::XMFLOAT4 toXMFLOAT4() const;
	btVector3 toBtVector3() const;

	void toQuaternion(float& q0, float& q1, float& q2, float& q3) const;

	//���K��
	Vec3 normalized() const;
	//�X�J���[
	float length() const;
	//�X�J���[�̓��
	float sqrLength() const;
	//��̃x�N�g���̋���
	float distance(const Vec3 & other) const;
	//��̃x�N�g���̋����̓��
	float sqrDistance(const Vec3 & other) const;

	//����
	float dot(const Vec3 & other) const;
	//�O��
	Vec3 cross(const Vec3 & other) const;

	//Vec3(0, 0, 0)
	static const Vec3& zero();
	//���`���
	static Vec3 lerp(const Vec3& from, const Vec3& to, float t);

	//��̃x�N�g���̋���
	static float distance(const Vec3 & v1, const Vec3 & v2);
	//��̃x�N�g���̋����̓��
	static float sqrDistance(const Vec3 & v1, const Vec3 & v2);

	//����
	static float dot(const Vec3 & v1, const Vec3 & v2);
	//�O��
	static Vec3 cross(const Vec3 & v1, const Vec3 & v2);

	Vec3 multMatrix(const DirectX::XMMATRIX& matrix);

#pragma region Operator
	const Vec3 operator +() const
	{
		return *this;
	}

	const Vec3 operator -() const
	{
		return *this * -1;
	}

	const Vec3 operator + (const Vec3& other) const
	{
		return Vec3(
			this->x + other.x,
			this->y + other.y,
			this->z + other.z
		);
	};

	const Vec3 operator - (const Vec3& other) const
	{
		return Vec3(
			this->x - other.x,
			this->y - other.y,
			this->z - other.z
		);
	};

	const Vec3 operator * (float n) const
	{
		return Vec3(
			this->x * n,
			this->y * n,
			this->z * n
		);
	}

	const Vec3 operator / (float n) const
	{
		if (n == 0)
			return Vec3(*this);

		return Vec3(
			this->x / n,
			this->y / n,
			this->z / n
		);
	}

	Vec3& operator += (const Vec3& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;

		return *this;
	}

	Vec3& operator -= (const Vec3& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;

		return *this;
	}

	Vec3& operator *= (float n)
	{
		this->x *= n;
		this->y *= n;
		this->z *= n;

		return *this;
	}

	Vec3& operator /= (float n)
	{
		this->x /= n;
		this->y /= n;
		this->z /= n;

		return *this;
	}

#pragma endregion

public:
	float x;
	float y;
	float z;

	static const Vec3 zeroVec;
};

