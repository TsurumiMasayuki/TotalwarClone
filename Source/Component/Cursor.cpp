#include "Cursor.h"
#include "Actor\Base\GameObject.h"
#include "Device\GameDevice.h"
#include "Def\Screen.h"
#include "Physics\IPhysicsManager.h"

void Cursor::init(Camera* pCamera)
{
	m_pCamera = pCamera;
}

void Cursor::onStart()
{
}

void Cursor::onUpdate()
{
	Vec3 mousePos = GameDevice::getInput().getMousePosition();
	//-1 ~ 1の値に変換
	mousePos.x = mousePos.x + WindowWidth * 0.5f;
	//-1 ~ 1の値に変換
	mousePos.y = -mousePos.y + WindowHeight * 0.5f;

	const auto& rotMat = m_pCamera->getTransform().getRotationMatrix();
	DirectX::XMVECTOR unproject1 = DirectX::XMVector3Unproject(
		mousePos.toXMVector(),
		0.0f, 0.0f, WindowWidth, WindowHeight,
		0.1f, 10.0f,
		m_pCamera->getProjMatrix(), m_pCamera->getViewMatrix(), DirectX::XMMatrixIdentity()
	);

	mousePos.z = 1.0f;
	DirectX::XMVECTOR unproject2 = DirectX::XMVector3Unproject(
		mousePos.toXMVector(),
		0.0f, 0.0f, WindowWidth, WindowHeight,
		0.1f, 10.0f,
		m_pCamera->getProjMatrix(), m_pCamera->getViewMatrix(), DirectX::XMMatrixIdentity()
	);

	Vec3 dir = Vec3(unproject2) - Vec3(unproject1);
	dir = dir.normalized();

	Vec3 origin = Vec3(unproject1);

	//カーソルがワールド上で指している場所を更新
	Vec3 forward(0.0f, 0.0f, 10.0f);
	RayHitResult result;
	getUser().getGameMediator()->getPhysicsManager()->raycastSingle(
		origin,
		origin + dir * 1000.0f,
		result
	);

	m_CursorRayPoint = result.hitPoint;
}

const Vec3& Cursor::getCursorPoint() const
{
	return m_CursorRayPoint;
}
