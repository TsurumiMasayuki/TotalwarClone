#pragma once
#include <vector>
#include <algorithm>
#include <Utility\INonCopyable.h>

//�����̃C���X�^���X���Ǘ����鎞�p�⏕�N���X
template<typename T>
class ManagerHelper
	: private INonCopyable
{
public:
	//�\�[�g���@���w�肵�Ēx���ǉ��pvector����������
	template<class ORDER_FUNC>
	void concatQueue(const ORDER_FUNC& orderFunc);

	//�S�ă\�[�g
	template<class ORDER_FUNC>
	void sort(const ORDER_FUNC& orderFunc);

	//�\�[�g���@���w�肵�Ă��̂܂ܒǉ�
	template<class ORDER_FUNC>
	void add(T value, const ORDER_FUNC& orderFunc);
	//�\�[�g���@���w�肹���ɒǉ�
	void add(T value);
	//�x���ǉ�
	void addQueue(T value);
	//�폜&���
	void remove(T value);
	//�S�č폜&���
	void removeAll();

	//�폜
	void erase(T value);
	//�S�č폜
	void eraseAll();

	//value�����݂��邩���ׂ�
	bool find(T value);

	//�x���ǉ�vector���擾
	std::vector<T>& getAddQueueVector();
	//vector���擾
	std::vector<T>& getVector();

	//�v�f��S�Ď擾
	const std::vector<T>& getAllEelements() const;

	//�֐���n���đS�Ď��s
	template<typename FUNC>
	void for_each(FUNC func);

private:
	std::vector<T> m_AddQueueVector;	//�x���ǉ��pvector
	std::vector<T> m_Vector;			//�{�Ǘ��pvector
	std::vector<T> m_AllElements;
};

template<typename T>
template<class ORDER_FUNC>
inline void ManagerHelper<T>::add(T value, const ORDER_FUNC & orderFunc)
{
	m_AllElements.emplace_back(value);

	//�\�[�g���@�ɏ]���Ď����̒l���傫�Ȓl��T��
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
	//�����ɍ폜�Ώۂ��ړ�����popback
	auto itr = std::find(m_AllElements.begin(), m_AllElements.end(), value);
	if (itr != m_AllElements.end())
	{
		std::iter_swap(itr, m_AllElements.end() - 1);
		m_AllElements.pop_back();
	}

	//�����ɍ폜�Ώۂ��ړ�����popback
	itr = std::find(m_Vector.begin(), m_Vector.end(), value);
	if (itr != m_Vector.end())
	{
		std::iter_swap(itr, m_Vector.end() - 1);
		delete *itr;
		m_Vector.pop_back();

		//��̏����͕K�v�Ȃ��̂�return;
		return;
	}

	//�����ɍ폜�Ώۂ��ړ�����popback
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
	//�����ɍ폜�Ώۂ��ړ�����popback
	auto itr = std::find(m_Vector.begin(), m_Vector.end(), value);
	if (itr != m_Vector.end())
	{
		std::iter_swap(itr, m_Vector.end() - 1);
		m_Vector.pop_back();

		//��̏����͕K�v�Ȃ��̂�return;
		return;
	}

	//�����ɍ폜�Ώۂ��ړ�����popback
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
	//�x���ǉ��pvector����Ȃ�I��
	if (m_AddQueueVector.empty()) return;

	for (auto& add : m_AddQueueVector)
	{
		//�\�[�g���@�ɏ]���Ď����̒l���傫�Ȓl��T��
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
	//���vector�Ŏ��s
	std::for_each(m_Vector.begin(), m_Vector.end(), func);
	std::for_each(m_AddQueueVector.begin(), m_AddQueueVector.end(), func);
}
