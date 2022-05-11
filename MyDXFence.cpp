#include "MyDXFence.h"
#include "Result.h"

void MyDXFence::Init(ID3D12Device* device)
{
	Result(device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
}

ID3D12Fence* MyDXFence::Fence()
{
	return fence;
}

UINT64 MyDXFence::Value()
{
	return fenceVal;
}

void MyDXFence::WaitCommandComplete(ID3D12CommandQueue* commandQueue)
{
	// コマンドの実行完了を待つ
	commandQueue->Signal(fence, ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}
