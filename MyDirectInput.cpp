#include "MyDirectInput.h"
#include "Result.h"

MyDirectInput* MyDirectInput::GetInstance()
{
	static MyDirectInput instance;
	return &instance;
}

void MyDirectInput::Init(const HINSTANCE hInstance, const HWND hwnd)
{
	// DirectInput 初期化
	Result(DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr));
	CreateKeyboard(hwnd);
}

void MyDirectInput::CreateKeyboard(const HWND hwnd)
{
	// キーボードデバイス 生成
	Result(directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL));

	// 入力データ形式セット
	Result(keyboard->SetDataFormat(&c_dfDIKeyboard));

	// 排他制御レベルセット
	Result(keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY));
}

void MyDirectInput::GetKeyboardState(BYTE keys[256])
{
	if (keys == nullptr) return;
	// キーボード情報の取得開始
	keyboard->Acquire();
	// 全キーの入力状態を取得する
	BYTE key[256];
	keyboard->GetDeviceState(sizeof(key), key);
	for (int i = 0; i < 256; i++)
	{
		keys[i] = key[i];
		keys[i] >>= 7;
	}
}

MyDirectInput::~MyDirectInput()
{
}
