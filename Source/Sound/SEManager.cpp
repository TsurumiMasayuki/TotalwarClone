#include "SEManager.h"
#include "Component\Utility\Transform.h"
#include "Component\Audio\AudioSource.h"
#include "Math\MathUtility.h"

void SEManager::update()
{
	const Vec3& listnerPosition = m_pListner->getLocalPosition();
	const float defaultDistance = 50.0f * 50.0f;

	//SE‚Ì‰¹—Ê‚ð‹——£‚É‰ž‚¶‚Ä’²®
	for (auto pSource : m_SESources)
	{
		const Vec3& sourcePosition = pSource->getTransform().getLocalPosition();
		float sqrDistance = listnerPosition.sqrDistance(sourcePosition);

		pSource->setVolume(MathUtility::clamp(defaultDistance / sqrDistance, 0.0f, 1.0f));
	}
}

void SEManager::addSESource(AudioSource* pSource)
{
	m_SESources.insert(pSource);
}

void SEManager::removeSESource(AudioSource* pSource)
{
	m_SESources.erase(pSource);
}

void SEManager::setListner(Transform* pListner)
{
	m_pListner = pListner;
}

void SEManager::clear()
{
	m_SESources.clear();
	m_pListner = nullptr;
}
