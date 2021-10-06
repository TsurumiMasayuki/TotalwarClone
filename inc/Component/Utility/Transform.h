#pragma once
#include <Component\Base\AbstractComponent.h>
#include <Utility\ManagerHelper.h>
#include <DirectXMath.h>
#include <Math\Vec3.h>

namespace DirectX
{
	struct XMMATRIX;
}

class ITransformEventListner;

class Transform
	: public AbstractComponent
{
public:
	Transform();
	~Transform();

	virtual void onStart() override {};
	virtual void onUpdate() override {};

	//ローカル座標の設定
	void setLocalPosition(const Vec3& position);
	//ローカル座標の取得
	const Vec3& getLocalPosition() const { return m_LocalPosition; }

	//ローカル角度の設定(Z)
	void setLocalAngleZ(float angle);
	//ローカル角度の取得(Z)
	float getLocalAngleZ() const { return m_LocalAngles.z; }

	//ローカル角度の設定
	void setLocalAngles(const Vec3& angles);
	//ローカル角度の取得
	const Vec3& getLocalAngles() const { return m_LocalAngles; }

	//ローカルスケールの設定
	void setLocalScale(const Vec3& scale);
	//ローカルスケールの取得
	const Vec3& getLocalScale() const { return m_LocalScale; }

	//ワールド行列の取得
	DirectX::XMMATRIX getWorldMatrix();
	//平行移動行列の取得
	const DirectX::XMMATRIX& getTranslateMatrix();
	//回転行列の取得
	const DirectX::XMMATRIX& getRotationMatrix();
	//拡大縮小行列の取得
	const DirectX::XMMATRIX& getScalingMatrix();

	//前方向のベクトル取得
	const Vec3& getForward() const;

	void addListner(ITransformEventListner* pListner);
	void removeListner(ITransformEventListner* pListner);

private:
	void onLocalPositionChanged(const Vec3& newLocalPosition);
	void onLocalScaleChanged(const Vec3& newLocalScale);
	void onLocalAnglesChanged(const Vec3& newLocalAngle);

	ManagerHelper<ITransformEventListner*> m_Listners;

private:
	Vec3 m_LocalPosition;
	Vec3 m_LocalAngles;
	Vec3 m_LocalScale;

	Vec3 m_Forward;

	DirectX::XMMATRIX m_Translate;
	DirectX::XMMATRIX m_Rotate;
	DirectX::XMMATRIX m_Scaling;
	DirectX::XMMATRIX m_Local;

	bool m_DirtyFlag;
};