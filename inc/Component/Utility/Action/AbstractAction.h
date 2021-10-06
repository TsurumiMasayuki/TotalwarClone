#pragma once
#include <Utility\INonCopyable.h>

class GameObject;
class Timer;

namespace Action
{
	class AbstractAction
		: private INonCopyable
	{
	public:
		AbstractAction(float time = 0.0f);
		virtual ~AbstractAction();

		virtual void setUseUnscaledTime(bool value);
		void setUser(GameObject* pUser) { m_pUser = pUser; }

		void baseInit();
		void baseUpdate();

		float getActionTime();

		virtual void init() = 0;				//������
		virtual void update(float time) = 0;	//�X�V
		virtual void onSuspend() = 0;			//���~��
		virtual bool isEnd();					//�I���������ǂ���

		virtual AbstractAction* clone() = 0;	//���g���R�s�[����


	protected:
		GameObject* m_pUser;
		Timer* m_pTimer;
	};
};