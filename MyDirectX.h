#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
//#include "MyDXAdapters.h"
//#include "MyDXDevice.h"
//#include "MyDXCommandList.h"
//#include "MyDXCommandQueue.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDirectX
{
private:
	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;
	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[4] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	//コマンドキュー設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// スワップチェーン設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	
	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;

public:
	MyDirectX();
	~MyDirectX();
private:
	void ListGraphicsBoardAdapters();
	void SelectGraphiscBoardAdapters();
	void CreateDevice();
	void CreateCommandList();
	void CreateCommandQueue();
	void SetSwapChain();
	void CreateSwapChain(HWND& hwnd);
	void SetDescriptorHeap();
	void CreateDescriptorHeap();
	void BackBaffer();
	void CreateRenderTargetView();
};

