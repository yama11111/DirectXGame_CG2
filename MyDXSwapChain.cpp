#include "MyDXSwapChain.h"
#include "Result.h"

void MyDXSwapChain::Init(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory, const HWND& hwnd)
{
	Set();
	Create(commandQueue, dxgiFactory, hwnd);
}

IDXGISwapChain4* MyDXSwapChain::SwapChain()
{
	return swapChain;
}

DXGI_SWAP_CHAIN_DESC1 MyDXSwapChain::SwapChainDesc()
{
	return swapChainDesc;
}

void MyDXSwapChain::FlipBuffer()
{
	// 画面に表示するバッファをフリップ(裏表の入替え)
	Result(swapChain->Present(1, 0));
}

void MyDXSwapChain::Set()
{
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		  // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1;						  // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;		  // バックバッファ用
	swapChainDesc.BufferCount = 2;							  // バッファ数を2つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
}

void MyDXSwapChain::Create(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory, const HWND& hwnd)
{
	// スワップチェーン生成
	Result(dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, hwnd, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain));
}
