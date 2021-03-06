#pragma once
#include <unordered_set>

#include "Utility\SingletonTemplate.h"

class Transform;
class AudioSource;

//効果音の管理
class SEManager
	: public SingletonTemplate<SEManager>
{
public:
	friend class SingletonTemplate<SEManager>;

	void update();

	//SEの発生源を追加
	void addSESource(AudioSource* pSource);
	//SEの発生源を削除
	void removeSESource(AudioSource* pSource);

	//SEの受信側を設定
	void setListner(Transform* pListner);

	//クリア
	void clear();

protected:
	SEManager() {};
	virtual ~SEManager() {};

private:
	std::unordered_set<AudioSource*> m_SESources;
	Transform* m_pListner;
};