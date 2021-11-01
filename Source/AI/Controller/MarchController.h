#pragma once
#include "AI\Controller\AbstractController.h"

//陣形移動モード
class MarchController
	: public AbstractController
{
public:
	MarchController(Unit* pUnit, IPlayer* pPlayer, const Vec3& relativePos, int unitWidth);

	virtual void start() override;
	virtual void update() override;

private:
	IPlayer* m_pPlayer;
	Unit* m_pUnit;
	Unit* m_pSlowestUnit;
	Vec3 m_RelativePos;
	int m_UnitWidth;
};