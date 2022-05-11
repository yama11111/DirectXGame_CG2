#include "MyDXRenderTargetView.h"

void MyDXRenderTargetView::Init(ID3D12Device* device, IDXGISwapChain4* swapChain, const DXGI_SWAP_CHAIN_DESC1& swapChainDesc)
{
	SetDescriptorHeap(swapChainDesc);
	CreateDescriptorHeap(device);
	SetBackBuffer(swapChainDesc);
	CreateRenderTargetView(device, swapChain);
}

std::vector<ID3D12Resource*> MyDXRenderTargetView::BackBuffers()
{
	return backBuffers;
}

D3D12_CPU_DESCRIPTOR_HANDLE MyDXRenderTargetView::Handle()
{
	return rtvHeap->GetCPUDescriptorHandleForHeapStart();
}

D3D12_DESCRIPTOR_HEAP_DESC MyDXRenderTargetView::Desc()
{
	return rtvHeapDesc;
}

void MyDXRenderTargetView::SetDescriptorHeap(const DXGI_SWAP_CHAIN_DESC1& swapChainDesc)
{
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ
}

void MyDXRenderTargetView::CreateDescriptorHeap(ID3D12Device* device)
{
	// デスクリプタヒープ生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
}

void MyDXRenderTargetView::SetBackBuffer(const DXGI_SWAP_CHAIN_DESC1& swapChainDesc)
{
	backBuffers.resize(swapChainDesc.BufferCount);
}

void MyDXRenderTargetView::CreateRenderTargetView(ID3D12Device* device, IDXGISwapChain4* swapChain)
{
	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// スワップチェーンからバッファ取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// デスクリプタヒープのハンドル取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲットビュー設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビュー生成
		device->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}
}
