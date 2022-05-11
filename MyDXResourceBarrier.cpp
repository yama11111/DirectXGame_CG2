#include "MyDXResourceBarrier.h"

void MyDXResourceBarrier::ChangeToDraw(ID3D12GraphicsCommandList* commandList,
				IDXGISwapChain4* swapChain, std::vector<ID3D12Resource*> backBuffers)
{
	// バックバッファの番号を取得(2つなので0番か1番)
	bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 1.リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex]; // バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	commandList->ResourceBarrier(1, &barrierDesc);
}

void MyDXResourceBarrier::ChangeToDisplay(ID3D12GraphicsCommandList* commandList)
{
	// 5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
	commandList->ResourceBarrier(1, &barrierDesc);
}

UINT MyDXResourceBarrier::BBIndex()
{
	return bbIndex;
}
