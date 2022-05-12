#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class MyDirectInput
{
private:
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;
public:
	static MyDirectInput* GetInstance();
	void Init(const HINSTANCE hInstance, const HWND hwnd);
	void GetKeyboardState(BYTE key[256]);
private:
	MyDirectInput() = default;
	~MyDirectInput();
	MyDirectInput(const MyDirectInput&) = delete;
	const MyDirectInput& operator=(const MyDirectInput&) = delete;
	void CreateKeyboard(const HWND hwnd);
};

