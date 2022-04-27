#include "MyDXCommandQueue.h"

void MyDXCommandQueue::Create(ID3D12Device*& device)
{
	HRESULT result;
	//コマンドキュー生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}
