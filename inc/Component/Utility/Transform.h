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

	//���[�J�����W�̐ݒ�
	void setLocalPosition(const Vec3& position);
	//���[�J�����W�̎擾
	const Vec3& getLocalPosition() const { return m_LocalPosition; }

	//���[�J���p�x�̐ݒ�(Z)
	void setLocalAngleZ(float angle);
	//���[�J���p�x�̎擾(Z)
	float getLocalAngleZ() const { return m_LocalAngles.z; }

	//���[�J���p�x�̐ݒ�
	void setLocalAngles(const Vec3& angles);
	//���[�J���p�x�̎擾
	const Vec3& getLocalAngles() const { return m_LocalAngles; }

	//���[�J���X�P�[���̐ݒ�
	void setLocalScale(const Vec3& scale);
	//���[�J���X�P�[���̎擾
	const Vec3& getLocalScale() const { return m_LocalScale; }

	//���[���h�s��̎擾
	DirectX::XMMATRIX getWorldMatrix();
	//���s�ړ��s��̎擾
	const DirectX::XMMATRIX& getTranslateMatrix();
	//��]�s��̎擾
	const DirectX::XMMATRIX& getRotationMatrix();
	//�g��k���s��̎擾
	const DirectX::XMMATRIX& getScalingMatrix();

	//�O�����̃x�N�g���擾
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