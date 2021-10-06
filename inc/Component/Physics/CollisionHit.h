#pragma once

template <class T>
class CollisionHit
{
public:
	CollisionHit(T* pCollider);
	~CollisionHit();

	void onDelete();

	void onAddRef();
	void onRemoveRef();

	bool isAccessable();

	T* getCollider();

private:
	T* m_pCollider;

	unsigned int m_RefCount;
	bool m_Deleted;
};

template<class T>
inline CollisionHit<T>::CollisionHit(T* pCollider)
	: m_pCollider(pCollider),
	m_Deleted(false),
	m_RefCount(0)
{
}

template<class T>
inline CollisionHit<T>::~CollisionHit()
{
}

template<class T>
inline void CollisionHit<T>::onDelete()
{
	m_Deleted = true;
	m_pCollider = nullptr;

	if (m_RefCount == 0)
		delete this;
}

template<class T>
inline void CollisionHit<T>::onAddRef()
{
	m_RefCount++;
}

template<class T>
inline void CollisionHit<T>::onRemoveRef()
{
	m_RefCount--;
	if (m_RefCount == 0 && m_Deleted)
		delete this;
}

template<class T>
inline bool CollisionHit<T>::isAccessable()
{
	return !m_Deleted;
}

template<class T>
inline T* CollisionHit<T>::getCollider()
{
	return m_pCollider;
}
