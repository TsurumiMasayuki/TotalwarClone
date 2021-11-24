#pragma once
#include <unordered_set>

#include "Utility\SingletonTemplate.h"

class Transform;
class AudioSource;

//Œø‰Ê‰¹‚ÌŠÇ—
class SEManager
	: public SingletonTemplate<SEManager>
{
public:
	friend class SingletonTemplate<SEManager>;

	void update();

	//SE‚Ì”­¶Œ¹‚ğ’Ç‰Á
	void addSESource(AudioSource* pSource);
	//SE‚Ì”­¶Œ¹‚ğíœ
	void removeSESource(AudioSource* pSource);

	//SE‚ÌóM‘¤‚ğİ’è
	void setListner(Transform* pListner);

	//ƒNƒŠƒA
	void clear();

protected:
	SEManager() {};
	virtual ~SEManager() {};

private:
	std::unordered_set<AudioSource*> m_SESources;
	Transform* m_pListner;
};