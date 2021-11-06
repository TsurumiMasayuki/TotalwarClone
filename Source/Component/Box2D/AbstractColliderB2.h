#pragma once
#include <Component\Base\AbstractComponent.h>
#include "box2d\b2_body.h"

class UnitObject;

class AbstractColliderB2
	: public AbstractComponent
{
public:
	AbstractColliderB2();
	virtual ~AbstractColliderB2();

	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	virtual void init() = 0;

	//速度を設定
	void setVelocity(float x, float z);

	//トリガーの状態設定
	void setTrigger(bool isTrigger);
	//トリガーの状態取得
	bool getTrigger() const;

	//グループを設定
	void setGroupIndex(int index);
	//BodyTypeの設定
	void setBodyType(const b2BodyType& bodyType);

	virtual void onLocalPositionChanged(const Vec3& newPosition) override;
	virtual void onLocalScaleChanged(const Vec3& newScale) override;

	virtual void onEnable() override;
	virtual void onDisable() override;

	void onCollisionEnter(AbstractColliderB2* pHit);
	void onCollisionStay(AbstractColliderB2* pHit);
	void onCollisionExit(AbstractColliderB2* pHit);

	void onTriggerEnter(AbstractColliderB2* pHit);
	void onTriggerStay(AbstractColliderB2* pHit);
	void onTriggerExit(AbstractColliderB2* pHit);

protected:
	void setPositionB2(const Vec3& position);
	Vec3 getPositionB2() const;

protected:
	bool m_IsTrigger;
	b2Body* m_pBody;
	UnitObject* m_pUnitObject;
	b2BodyType m_BodyType;
	int m_GroupIndex;
};