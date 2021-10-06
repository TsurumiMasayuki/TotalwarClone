#pragma once
#include <DirectXMath.h>
#include <LinearMath\btVector3.h>

struct Vec3
{
public:
	explicit Vec3();
	//vをx, y, z全てに代入
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

	//正規化
	Vec3 normalized() const;
	//スカラー
	float length() const;
	//スカラーの二乗
	float sqrLength() const;
	//二つのベクトルの距離
	float distance(const Vec3 & other) const;
	//二つのベクトルの距離の二乗
	float sqrDistance(const Vec3 & other) const;

	//内積
	float dot(const Vec3 & other) const;
	//外積
	Vec3 cross(const Vec3 & other) const;

	//Vec3(0, 0, 0)
	static const Vec3& zero();
	//線形補間
	static Vec3 lerp(const Vec3& from, const Vec3& to, float t);

	//二つのベクトルの距離
	static float distance(const Vec3 & v1, const Vec3 & v2);
	//二つのベクトルの距離の二乗
	static float sqrDistance(const Vec3 & v1, const Vec3 & v2);

	//内積
	static float dot(const Vec3 & v1, const Vec3 & v2);
	//外積
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

