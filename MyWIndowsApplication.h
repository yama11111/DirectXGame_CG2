#pragma once
#include <Windows.h>
#include "Component.h"

class MyWIndowsApplication
{
public:
	// ウィンドウサイズ 
	const Component* SIZE; // (横, 縦)
private:
	// ウィンドウクラス設定
	WNDCLASSEX w{};
	// ウィンドウオブジェクト
	HWND hwnd;
	MSG msg{}; // メッセージ
public:
	MyWIndowsApplication();
	~MyWIndowsApplication();
	void CreateMyWindow(const wchar_t* titleName = L"DirectXGame", const Component& windowSize = { 1280, 720 });
	bool MessageLoop();
	void FinalProcess();
	HWND HandleWindow();
	HINSTANCE HandleWindowInstance();
	//ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

