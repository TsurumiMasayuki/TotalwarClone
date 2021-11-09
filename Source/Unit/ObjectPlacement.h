#pragma once
#include <unordered_set>
#include <Math\Vec3.h>

class Transform;

//���j�b�g���̃I�u�W�F�N�g�̔z�u�S��
class ObjectPlacement
{
public:
	ObjectPlacement();
	
	//�I�u�W�F�N�g�̍��W�v�Z
	void calculateObjectPositions(std::vector<Vec3>& results);
	//�I�u�W�F�N�g�̍��W�ݒ�
	void applyObjectPositions();

	//�I�u�W�F�N�g�o�^
	void addObject(Transform* pObject);
	//�I�u�W�F�N�g�폜
	void removeObject(Transform* pObject);

	void setBasePos(const Vec3& basePos);
	const Vec3& getBasePos() const;

	void setWidth(int width);
	int getWidth() const;

	void setSpacePerObject(float spacePerObject);
	float getSpacePerObject() const;

	void setAngle(float angle);
	float getAngle() const;

private:
	//�I�u�W�F�N�g���X�g
	std::unordered_set<Transform*> m_ObjectList;

	//���S���W
	Vec3 m_BasePos;
	//��
	int m_Width;
	//�I�u�W�F�N�g���Ƃ̔z�u�Ԋu
	float m_SpacePerObject;
	//�z�u�p�x
	float m_Radian;
};