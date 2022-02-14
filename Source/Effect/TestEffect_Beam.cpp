#include "TestEffect_Beam.h"
#include "Actor\Base\GameObject.h"
#include "Component\Audio\AudioSource.h"
#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"
#include "Math\MathUtility.h"
#include "Utility\Random.h"

#include "Sound\SEManager.h"

using namespace Action;

void TestEffect_Beam::onStart()
{
	m_pBeamTransform = &getUser().getTransform();
	m_pActionManager = getUser().addComponent<ActionManager>();

	m_pBeamTransform->setLocalScale(Vec3::zero());

	m_pSEAudio = getUser().addComponent<AudioSource>();
	m_pSEAudio->setAudio("Shot1");

	SEManager::getInstance().addSESource(m_pSEAudio);
}

void TestEffect_Beam::onUpdate()
{
	m_pEffectRenderHelper->appendInstanceInfoSingle(m_pBeamTransform->getWorldMatrix(), m_Color);
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

void TestEffect_Beam::init(EffectRenderHelper* pEffectRenderHelper, const Color& color)
{
	m_pEffectRenderHelper = pEffectRenderHelper;
	m_Color = color;
}

void TestEffect_Beam::playEffect()
{
	//エフェクト実行中なら何もしない
	if (m_pActionManager->actionCount() > 0) 
		return;

	Vec3 diff = m_EndPos - m_BeginPos;
	float radian = atan2f(diff.x, diff.z);
	float angle = MathUtility::toDegree(radian);
	float distance = diff.length();

	m_pBeamTransform->setLocalPosition(m_BeginPos);
	m_pBeamTransform->setLocalScale(Vec3(0.0f));
	m_pBeamTransform->setLocalAngles(Vec3(0.0f, angle, 0.0f));

	auto pBeamAnimation = new Spawn(
		{
			new MoveBy(diff, m_Time),
			new Sequence(
				{
					new ScaleBy(Vec3(m_Width, m_Width, distance * 0.01f), m_Time * 0.5f),
					new ScaleBy(Vec3(-m_Width, -m_Width, -distance * 0.01f), m_Time * 0.5f)
				}
			)
		}
	);

	m_pActionManager->enqueueAction(pBeamAnimation);

	//効果音を再生
	m_pSEAudio->play();
	m_pSEAudio->setPitch(GameDevice::getRandom().getRandom(0.1f, 1.0f));
}