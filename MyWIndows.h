#pragma once
#include <windows.h>
#include "Vec2.h"

class MyWindows
{
private:
	// ウィンドウクラス設定
	WNDCLASSEX w{};
	// ウィンドウオブジェクト
	HWND hwnd;
	// メッセージ
	MSG msg{};
public:
	static MyWindows* GetInstance();
	//ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void Create(const wchar_t* titleName = L"DirectXGame",
		const Vec2& winSize = { 1280, 720 });
	bool CheckMessage();
	void FinalProcess();
	HWND HandleWindow();
	HINSTANCE HandleWindowInstance();
private:
	MyWindows() = default;
	~MyWindows() = default;
	MyWindows(const MyWindows&) = delete;
	const MyWindows& operator=(const MyWindows&) = delete;

};
