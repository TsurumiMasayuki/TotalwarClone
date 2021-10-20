#pragma once
#pragma once

//�V���O���g���e���v���[�g
template<typename T>
class SingletonTemplate
{
public:
	//�C���X�^���X�̎擾
	static T& getInstance();

protected:
	//�R���X�g���N�^���B��
	SingletonTemplate() {}
	//�f�X�g���N�^���B��
	virtual ~SingletonTemplate() {}

private:
	//������Z�q�֎~
	void operator = (const SingletonTemplate&) = delete;
	//�R�s�[�R���X�g���N�^�֎~
	SingletonTemplate(const SingletonTemplate&) = delete;
};

template<typename T>
inline T& SingletonTemplate<T>::getInstance()
{
	static T instance;
	return instance;
}