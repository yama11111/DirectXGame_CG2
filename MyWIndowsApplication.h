#pragma once
#include <Windows.h>
#include "Component.h"

class MyWIndowsApplication
{
public:
	// �E�B���h�E�T�C�Y 
	const Component* SIZE; // (��, �c)
private:
	// �E�B���h�E�N���X�ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�I�u�W�F�N�g
	HWND hwnd;
	MSG msg{}; // ���b�Z�[�W
public:
	MyWIndowsApplication();
	~MyWIndowsApplication();
	void CreateMyWindow(const wchar_t* titleName = L"DirectXGame", const Component& windowSize = { 1280, 720 });
	bool MessageLoop();
	void FinalProcess();
	HWND HandleWindow();
	HINSTANCE HandleWindowInstance();
	//�E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

