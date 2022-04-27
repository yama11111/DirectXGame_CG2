#pragma once
//#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
//#include <string>

//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")

class MyDXAdapters
{
private:
	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;
public:
	MyDXAdapters();
	~MyDXAdapters();
private:
	void List(IDXGIFactory7*& dxgiFactory);
	void Select();
};

