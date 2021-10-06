#pragma once
#include <AbstractGame.h>

class TotalwarCloneGame
	: public AbstractGame
{
public:
	TotalwarCloneGame(HINSTANCE& hinstance, HWND& hwnd);

	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onShutdown() override;
};