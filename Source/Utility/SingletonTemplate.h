#pragma once
#pragma once

//シングルトンテンプレート
template<typename T> class SingletonTemplate
{
public:
	//インスタンスの取得
	static T& getInstance();

protected:
	//コンストラクタを隠す
	SingletonTemplate() {}
	//デストラクタを隠す
	virtual ~SingletonTemplate() {}

private:
	//代入演算子禁止
	void operator = (const SingletonTemplate&) = delete;
	//コピーコンストラクタ禁止
	SingletonTemplate(const SingletonTemplate&) = delete;
};

template<typename T>
inline T& SingletonTemplate<T>::getInstance()
{
	static T instance;
	return instance;
}