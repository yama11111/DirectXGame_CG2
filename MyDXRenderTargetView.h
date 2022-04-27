#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>

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
	MyDXRenderTargetView();
	~MyDXRenderTargetView();
private:
	void SetDescriptorHeap();
	void CreateDescriptorHeap();
	void SetBackBaffer();
	void CreateRenderTargetView();
};

