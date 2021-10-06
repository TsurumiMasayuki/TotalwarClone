#pragma once

//�R�s�[�֎~�p�N���X private�p������ƃN���X�̃R�s�[���֎~�ł��܂�
class INonCopyable
{
protected:
	INonCopyable() {};
	virtual ~INonCopyable() {};

private:
	//�R�s�[�֎~
	void operator =(const INonCopyable& src) = delete;
	INonCopyable(const INonCopyable& src) = delete;
};