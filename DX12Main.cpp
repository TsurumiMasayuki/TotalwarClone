#include <Windows.h>
#include "TotalwarCloneGame.h"

#include <crtdbg.h>

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:	//ウィンドウが破棄された
		PostQuitMessage(0);	//OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);	//標準の処理を行う
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WNDCLASSEX wndClass{};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WndProc;			//ウィンドウプロシージャを設定
	wndClass.lpszClassName = L"DirectXGame";				//ウィンドウクラス名
	wndClass.hInstance = GetModuleHandle(nullptr);		//ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&wndClass);
	//ウィンドウサイズ{X,Y,横幅,縦幅}
	RECT wrc = { 0, 0, 1280, 720 };
	//自動でサイズ補正
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウクラスの作成
	HWND hwnd = CreateWindow(
		wndClass.lpszClassName,		//クラス名
		L"DirectXGame",				//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,		//標準的なウィンドウスタイル
		CW_USEDEFAULT,				//X座標(OSに任せる)
		CW_USEDEFAULT,				//Y座標(OSに任せる)
		wrc.right - wrc.left,		//ウィンドウ横幅
		wrc.bottom - wrc.top,		//ウィンドウ縦幅
		nullptr,					//親ウィンドウハンドル
		nullptr,					//メニューハンドル
		wndClass.hInstance,			//呼び出しアプリケーションハンドル
		nullptr);					//オプション

	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	TotalwarCloneGame game(wndClass.hInstance, hwnd);
	game.start();

	MSG msg{};	//メッセージ

	while (true)
	{
		//メッセージの確認
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);	//キー入力メッセージの処理
			DispatchMessage(&msg);	//プロシージャにメッセージを送る
		}

		//終了メッセージが来たらループを抜ける
		if (msg.message == WM_QUIT)
		{
			break;
		}

		//毎フレーム処理
		game.update();

		//ゲーム終了してるならループ終了
		if (game.isEnd()) break;
	}

	//終了処理
	game.shutdown();
	GameDevice::getModelManager().endLoading();
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);

	return 0;
}