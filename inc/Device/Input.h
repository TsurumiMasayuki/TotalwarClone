#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Math\Vec3.h>

#pragma comment(lib, "dinput8.lib")

class Input
{
public:
	void init(const HINSTANCE& hinstance, const HWND& hwnd);
	void update();
	void shutdown();

	bool isKey(int keyNumber) const;
	bool isKeyUp(int keyNumber) const;
	bool isKeyDown(int keyNumber) const;

	bool isMouseButton(int keyNumber) const;
	bool isMouseButtonUp(int keyNumber) const;
	bool isMouseButtonDown(int keyNumber) const;

	Vec3 getMousePosition() const;
	Vec3 getMouseMove() const;

	//マウスホイールの動きを返します
	float getMouseWheelMove() const;

private:
	IDirectInput8* m_pDInput;
	IDirectInputDevice8* m_pDevKeyBoard;
	IDirectInputDevice8* m_pDevMouse;

	DIMOUSESTATE m_PreMouseState;
	DIMOUSESTATE m_CurMouseState;

	HWND m_HWND;

	BYTE m_PreKey[256];
	BYTE m_CurKey[256];
};