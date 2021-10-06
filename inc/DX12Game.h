#pragma once
#include <AbstractGame.h>

class DX12Game
	: public AbstractGame
{
public:
	DX12Game(HINSTANCE& hinstance, HWND& hwnd);

protected:
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onShutdown() override;
};