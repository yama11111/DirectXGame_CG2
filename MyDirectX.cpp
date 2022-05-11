#include "MyDirectX.h"
#include "Result.h"

MyDirectX::MyDirectX()
{
}

MyDirectX::~MyDirectX()
{
}

void MyDirectX::Init(const HWND& hwnd)
{
	myAdpts.Init();
	myDvc.Init(myAdpts.Template());
	myCmdList.Init(myDvc.Device());
	myCmdQueue.Init(myDvc.Device());
	mySwapChain.Init(myCmdQueue.CommandQueue(),myAdpts.Factory(), hwnd);
	myRTV.Init(myDvc.Device(), mySwapChain.SwapChain(), mySwapChain.SwapChainDesc());
	myFence.Init(myDvc.Device());
}

void MyDirectX::PreDraw()
{
	myRscBarrier.ChangeToDraw(myCmdList.CommandList(), mySwapChain.SwapChain(), myRTV.BackBuffers());

	// 2.描画先の変更
	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = myRTV.Handle();
	rtvHandle.ptr += myRscBarrier.BBIndex() * myDvc.Device()->GetDescriptorHandleIncrementSize(myRTV.Desc().Type);
	myCmdList.CommandList()->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	// 3.画面クリア {R, G, B, A}
	myCmdList.ClearScreen(rtvHandle, { 1.0f, 1.0f, 1.0f, 1.0f }); // 青っぽい色
}

void MyDirectX::PostDraw()
{
	myRscBarrier.ChangeToDisplay(myCmdList.CommandList());

	// 命令のクローズ
	myCmdList.CloseCommand();
	// コマンドリスト実行
	myCmdQueue.ExeCommmand(1, myCmdList.CommandList());
	// 画面に表示するバッファをフリップ(裏表の入替え)
	mySwapChain.FlipBuffer();

	// コマンドの実行完了を待つ
	myFence.WaitCommandComplete(myCmdQueue.CommandQueue());
	// キューをクリア
	myCmdList.ClearQueue();
	// 再びコマンドリストを貯める準備
	myCmdList.PreStore();

}
