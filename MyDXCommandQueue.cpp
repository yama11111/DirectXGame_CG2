#include "MyDXCommandQueue.h"

void MyDXCommandQueue::Create(ID3D12Device*& device)
{
	HRESULT result;
	//�R�}���h�L���[����
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}
