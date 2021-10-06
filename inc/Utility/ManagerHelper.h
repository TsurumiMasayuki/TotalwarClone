#pragma once
#include <vector>
#include <algorithm>
#include <Utility\INonCopyable.h>

//多数のインスタンスを管理する時用補助クラス
template<typename T>
class ManagerHelper
	: private INonCopyable
{
public:
	//ソート方法を指定して遅延追加用vectorを結合する
	template<class ORDER_FUNC>
	void concatQueue(const ORDER_FUNC& orderFunc);

	//全てソート
	template<class ORDER_FUNC>
	void sort(const ORDER_FUNC& orderFunc);

	//ソート方法を指定してそのまま追加
	template<class ORDER_FUNC>
	void add(T value, const ORDER_FUNC& orderFunc);
	//ソート方法を指定せずに追加
	void add(T value);
	//遅延追加
	void addQueue(T value);
	//削除&解放
	void remove(T value);
	//全て削除&解放
	void removeAll();

	//削除
	void erase(T value);
	//全て削除
	void eraseAll();

	//valueが存在するか調べる
	bool find(T value);

	//遅延追加vectorを取得
	std::vector<T>& getAddQueueVector();
	//vectorを取得
	std::vector<T>& getVector();

	//要素を全て取得
	const std::vector<T>& getAllEelements() const;

	//関数を渡して全て実行
	template<typename FUNC>
	void for_each(FUNC func);

private:
	std::vector<T> m_AddQueueVector;	//遅延追加用vector
	std::vector<T> m_Vector;			//本管理用vector
	std::vector<T> m_AllElements;
};

template<typename T>
template<class ORDER_FUNC>
inline void ManagerHelper<T>::add(T value, const ORDER_FUNC & orderFunc)
{
	m_AllElements.emplace_back(value);

	//ソート方法に従って自分の値より大きな値を探す
	auto find = std::upper_bound(m_Vector.begin(), m_Vector.end(), value, orderFunc);
	if (m_Vector.size() == 0)
	{
		m_Vector.emplace_back(value);
		return;
	}
	m_Vector.insert(find, value);
}

template<typename T>
inline void ManagerHelper<T>::add(T value)
{
	m_AllElements.emplace_back(value);
	m_Vector.emplace_back(value);
}

template<typename T>
inline void ManagerHelper<T>::addQueue(T value)
{
	m_AllElements.emplace_back(value);
	m_AddQueueVector.emplace_back(value);
}

template<typename T>
inline void ManagerHelper<T>::remove(T value)
{
	//末尾に削除対象を移動してpopback
	auto itr = std::find(m_AllElements.begin(), m_AllElements.end(), value);
	if (itr != m_AllElements.end())
	{
		std::iter_swap(itr, m_AllElements.end() - 1);
		m_AllElements.pop_back();
	}

	//末尾に削除対象を移動してpopback
	itr = std::find(m_Vector.begin(), m_Vector.end(), value);
	if (itr != m_Vector.end())
	{
		std::iter_swap(itr, m_Vector.end() - 1);
		delete *itr;
		m_Vector.pop_back();

		//後の処理は必要ないのでreturn;
		return;
	}

	//末尾に削除対象を移動してpopback
	itr = std::find(m_AddQueueVector.begin(), m_AddQueueVector.end(), value);
	if (itr != m_AddQueueVector.end())
	{
		std::iter_swap(itr, m_AddQueueVector.end() - 1);
		delete *itr;
		m_AddQueueVector.pop_back();
	}
}

template<typename T>
inline void ManagerHelper<T>::removeAll()
{
	while (!m_Vector.empty())
	{
		remove(m_Vector.back());
	}

	while (!m_AddQueueVector.empty())
	{
		remove(m_AddQueueVector.back());
	}

	m_AddQueueVector.clear();
}

template<typename T>
inline void ManagerHelper<T>::erase(T value)
{
	//末尾に削除対象を移動してpopback
	auto itr = std::find(m_Vector.begin(), m_Vector.end(), value);
	if (itr != m_Vector.end())
	{
		std::iter_swap(itr, m_Vector.end() - 1);
		m_Vector.pop_back();

		//後の処理は必要ないのでreturn;
		return;
	}

	//末尾に削除対象を移動してpopback
	itr = std::find(m_AddQueueVector.begin(), m_AddQueueVector.end(), value);
	if (itr != m_AddQueueVector.end())
	{
		std::iter_swap(itr, m_AddQueueVector.end() - 1);
		m_AddQueueVector.pop_back();
	}
}

template<typename T>
inline void ManagerHelper<T>::eraseAll()
{
	while (!m_Vector.empty())
	{
		erase(m_Vector.back());
	}

	while (!m_AddQueueVector.empty())
	{
		erase(m_AddQueueVector.back());
	}

	m_AddQueueVector.clear();
}

template<typename T>
inline bool ManagerHelper<T>::find(T value)
{
	return std::count_if(m_AllElements.begin(), m_AllElements.end(), value) != m_AllElements.end();
}

template<typename T>
inline std::vector<T>& ManagerHelper<T>::getAddQueueVector()
{
	return m_AddQueueVector;
}

template<typename T>
inline std::vector<T>& ManagerHelper<T>::getVector()
{
	return m_Vector;
}

template<typename T>
inline const std::vector<T>& ManagerHelper<T>::getAllEelements() const
{
	return m_AllElements;
}

template<typename T>
template<class ORDER_FUNC>
inline void ManagerHelper<T>::concatQueue(const ORDER_FUNC& orderFunc)
{
	//遅延追加用vectorが空なら終了
	if (m_AddQueueVector.empty()) return;

	for (auto& add : m_AddQueueVector)
	{
		//ソート方法に従って自分の値より大きな値を探す
		auto find = std::upper_bound(m_Vector.begin(), m_Vector.end(), add, orderFunc);
		m_Vector.insert(find, add);
	}

	m_AddQueueVector.clear();
	m_AddQueueVector.shrink_to_fit();
}

template<typename T>
template<class ORDER_FUNC>
inline void ManagerHelper<T>::sort(const ORDER_FUNC & orderFunc)
{
	std::sort(m_Vector.begin(), m_Vector.end(), orderFunc);
}

template<typename T>
template<typename FUNC>
inline void ManagerHelper<T>::for_each(FUNC func)
{
	//二つのvectorで実行
	std::for_each(m_Vector.begin(), m_Vector.end(), func);
	std::for_each(m_AddQueueVector.begin(), m_AddQueueVector.end(), func);
}
