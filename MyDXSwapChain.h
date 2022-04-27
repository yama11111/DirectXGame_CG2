#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDXSwapChain
{
private:
	IDXGISwapChain4* swapChain = nullptr;
	// スワップチェーン設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
public:
	MyDXSwapChain();
	~MyDXSwapChain();
	void Init();
private:
	void Create(ID3D12CommandQueue*& commandQueue, IDXGIFactory7*& dxgiFactory, HWND& hwnd);
};

