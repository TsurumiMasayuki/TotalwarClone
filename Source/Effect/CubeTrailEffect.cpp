#include "CubeTrailEffect.h"
#include "Actor\Base\GameObject.h"
#include "Component\Utility\Action\Actions.h"
#include "Component\Utility\Action\ActionManager.h"

void CubeTrailEffect::onStart()
{
	m_Timer.setMaxTime(1.0f);
}

void CubeTrailEffect::onUpdate()
{
	m_Timer.update();

	//Žw’èŽžŠÔ‚Å‚È‚©‚Á‚½‚çI—¹
	if (!m_Timer.isTime()) return;

	m_Timer.reset();

	auto pEffectObject = new GameObject(getUser().getGameMediator());
	pEffectObject->getTransform().setLocalPosition(getTransform().getLocalPosition() - Vec3(0.0f, 0.5f, 0.0f));
	pEffectObject->getTransform().setLocalScale(Vec3::zero());

	auto pCubeTrailParticle = pEffectObject->addComponent<CubeTrailParticle>();
	pCubeTrailParticle->init(m_pEffectRenderHelper);

	auto pActionManager = pEffectObject->addComponent<Action::ActionManager>();
	//‘å‚«‚­‚·‚é
	pActionManager->enqueueAction(new Action::EaseInQuint(new Action::ScaleTo(Vec3(0.01f), 1.0f)));
	//‚¾‚ñ‚¾‚ñ¬‚³‚­‚·‚é
	pActionManager->enqueueAction(new Action::ScaleTo(Vec3::zero(), 2.0f));
	//”jŠü
	pActionManager->enqueueAction(new Action::Destroy(0.0f));
}

void CubeTrailEffect::init(EffectRenderHelper* pEffectRenderHelper)
{
	m_pEffectRenderHelper = pEffectRenderHelper;
}

void CubeTrailParticle::onStart()
{
}

void CubeTrailParticle::onUpdate()
{
	m_pEffectRenderHelper->appendInstanceInfoSingle(getTransform().getWorldMatrix(), Color(DirectX::Colors::Orange));
}

void CubeTrailParticle::init(EffectRenderHelper* pEffectRenderHelper)
{
	m_pEffectRenderHelper = pEffectRenderHelper;
}
