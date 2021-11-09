#pragma once
#include <unordered_set>
#include <Math\Vec3.h>

class Transform;

//ユニット内のオブジェクトの配置担当
class ObjectPlacement
{
public:
	ObjectPlacement();
	
	//オブジェクトの座標計算
	void calculateObjectPositions(std::vector<Vec3>& results);
	//オブジェクトの座標設定
	void applyObjectPositions();

	//オブジェクト登録
	void addObject(Transform* pObject);
	//オブジェクト削除
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
	//オブジェクトリスト
	std::unordered_set<Transform*> m_ObjectList;

	//中心座標
	Vec3 m_BasePos;
	//幅
	int m_Width;
	//オブジェクトごとの配置間隔
	float m_SpacePerObject;
	//配置角度
	float m_Radian;
};