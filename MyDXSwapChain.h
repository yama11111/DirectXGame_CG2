#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDXSwapChain
{
private:
	IDXGISwapChain4* swapChain = nullptr;
	// スワップチェーン設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
public:
	void Init(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory, const HWND& hwnd);
	IDXGISwapChain4* SwapChain();
	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc();
	void FlipBuffer();
private:
	void Set();
	void Create(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory, const HWND& hwnd);
};

