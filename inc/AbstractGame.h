#pragma once
#include <Windows.h>
#include <Device\GameDevice.h>
#include <Graphics\DX12\DX12GraphicsCore.h>
#include <Scene\Base\SceneManager.h>

class AbstractGame
{
public:
	AbstractGame(HINSTANCE& hinstance, HWND& hwnd)
		: m_HINSTANCE(hinstance),
		m_HWND(hwnd),
		m_IsEnd(false)
	{
	}

	void start();
	void update();
	void shutdown();

	bool isEnd() const { return m_IsEnd; }

protected:
	virtual void onStart() = 0;
	virtual void onUpdate() = 0;
	virtual void onShutdown() = 0;

protected:
	SceneManager m_SceneManager;
	HINSTANCE m_HINSTANCE;
	HWND m_HWND;

	bool m_IsEnd;
};