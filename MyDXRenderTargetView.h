#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDXRenderTargetView
{
private:
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	// デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;
public:
	void Init(ID3D12Device* device, IDXGISwapChain4* swapChain, const DXGI_SWAP_CHAIN_DESC1& swapChainDesc);
	std::vector<ID3D12Resource*> BackBuffers();
	D3D12_CPU_DESCRIPTOR_HANDLE Handle();
	D3D12_DESCRIPTOR_HEAP_DESC Desc();
private:
	void SetDescriptorHeap(const DXGI_SWAP_CHAIN_DESC1& swapChainDesc);
	void CreateDescriptorHeap(ID3D12Device* device);
	void SetBackBuffer(const DXGI_SWAP_CHAIN_DESC1& swapChainDesc);
	void CreateRenderTargetView(ID3D12Device* device, IDXGISwapChain4* swapChain);
};

