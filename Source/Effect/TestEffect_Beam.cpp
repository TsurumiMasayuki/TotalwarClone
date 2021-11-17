#include "TestEffect_Beam.h"
#include "Actor\Base\GameObject.h"
#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"
#include "Math\MathUtility.h"

using namespace Action;

void TestEffect_Beam::onStart()
{
	m_pBeamTransform = &getUser().getTransform();
	m_pActionManager = getUser().addComponent<ActionManager>();

	m_pBeamTransform->setLocalScale(Vec3::zero());
}

void TestEffect_Beam::onUpdate()
{
	m_pEffectRenderHelper->appendInstanceInfoSingle(m_pBeamTransform->getWorldMatrix());
}

void TestEffect_Beam::setBeginPos(const Vec3& position)
{
	m_BeginPos = position;
}

void TestEffect_Beam::setEndPos(const Vec3& position)
{
	m_EndPos = position;
}

void TestEffect_Beam::setWidth(float width)
{
	m_Width = width * 0.01f;
}

void TestEffect_Beam::setTime(float time)
{
	m_Time = time;
}

void TestEffect_Beam::init(EffectRenderHelper* pEffectRenderHelper)
{
	m_pEffectRenderHelper = pEffectRenderHelper;
}

void TestEffect_Beam::playEffect()
{
	//�A�N�V�������s���Ȃ牽�����Ȃ�
	if (m_pActionManager->actionCount() > 0) 
		return;

	Vec3 diff = m_EndPos - m_BeginPos;
	float radian = atan2f(diff.x, diff.z);
	float angle = MathUtility::toDegree(radian);
	float distance = diff.length();

	m_pBeamTransform->setLocalPosition(m_BeginPos + diff.normalized() * distance * 0.5f);
	m_pBeamTransform->setLocalScale(Vec3(0.0f, 0.0f, distance * 0.01f));
	m_pBeamTransform->setLocalAngles(Vec3(0.0f, angle, 0.0f));

	m_pActionManager->enqueueAction(new ScaleBy(Vec3(m_Width, m_Width, 0.0f), m_Time * 0.33f));
	m_pActionManager->enqueueAction(new WaitForSeconds(m_Time * 0.33f));
	m_pActionManager->enqueueAction(new ScaleBy(Vec3(-m_Width, -m_Width, 0.0f), m_Time * 0.33f));
}