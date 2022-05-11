#pragma once
#include <dxgi1_6.h>
#include <vector>
#pragma comment(lib, "dxgi.lib")

class MyDXAdapters
{
private:
	IDXGIFactory7* dxgiFactory = nullptr;
	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;
public:
	void Init();
	IDXGIAdapter4* Template();
	IDXGIFactory7* Factory();
private:
	void List();
	void Select();
};

