#pragma once
#include <vector>


class GameObject;
class IGameMediator;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void update();
	void shutdown();
	void add(GameObject* pGameObject);
	void remove(GameObject* pGameObject);

	std::vector<GameObject*> getAllGameObject() const;

private:
	void addObjects();
	void destroyObjects();

private:
	std::vector<GameObject*> m_GameObjects;
	std::vector<GameObject*> m_AddObjects;

	int m_LatestObjectNumber;
};

