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

		virtual void init() = 0;				//初期化
		virtual void update(float time) = 0;	//更新
		virtual void onSuspend() = 0;			//中止時
		virtual bool isEnd();					//終了したかどうか

		virtual AbstractAction* clone() = 0;	//自身をコピーする


	protected:
		GameObject* m_pUser;
		Timer* m_pTimer;
	};
};