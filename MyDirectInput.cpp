#include "MyDirectInput.h"
#include "Result.h"

MyDirectInput* MyDirectInput::GetInstance()
{
	static MyDirectInput instance;
	return &instance;
}

void MyDirectInput::Init(const HINSTANCE hInstance, const HWND hwnd)
{
	// DirectInput ������
	Result(DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr));
	CreateKeyboard(hwnd);
}

void MyDirectInput::CreateKeyboard(const HWND hwnd)
{
	// �L�[�{�[�h�f�o�C�X ����
	Result(directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL));

	// ���̓f�[�^�`���Z�b�g
	Result(keyboard->SetDataFormat(&c_dfDIKeyboard));

	// �r�����䃌�x���Z�b�g
	Result(keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY));
}

void MyDirectInput::GetKeyboardState(BYTE keys[256])
{
	if (keys == nullptr) return;
	// �L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	// �S�L�[�̓��͏�Ԃ��擾����
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
