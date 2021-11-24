#pragma once
#include <unordered_set>

#include "Utility\SingletonTemplate.h"

class Transform;
class AudioSource;

//���ʉ��̊Ǘ�
class SEManager
	: public SingletonTemplate<SEManager>
{
public:
	friend class SingletonTemplate<SEManager>;

	void update();

	//SE�̔�������ǉ�
	void addSESource(AudioSource* pSource);
	//SE�̔��������폜
	void removeSESource(AudioSource* pSource);

	//SE�̎�M����ݒ�
	void setListner(Transform* pListner);

	//�N���A
	void clear();

protected:
	SEManager() {};
	virtual ~SEManager() {};

private:
	std::unordered_set<AudioSource*> m_SESources;
	Transform* m_pListner;
};