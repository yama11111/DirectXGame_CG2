#pragma once
#include <windows.h>
#include "Vec2.h"

class MyWindows
{
private:
	// �E�B���h�E�N���X�ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�I�u�W�F�N�g
	HWND hwnd;
	// ���b�Z�[�W
	MSG msg{};
public:
	static MyWindows* GetInstance();
	//�E�B���h�E�v���V�[�W��
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
