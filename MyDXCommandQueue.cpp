#include "MyDXCommandQueue.h"
#include "Result.h"

void MyDXCommandQueue::Init(ID3D12Device* device)
{
	if (device == nullptr) return;
	//�R�}���h�L���[����
	Result(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue)));
}

ID3D12CommandQueue* MyDXCommandQueue::CommandQueue()
{
	return commandQueue;
}

void MyDXCommandQueue::ExeCommmand(const int ary, ID3D12CommandList* commandList)
{
	// �R�}���h���X�g���s
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(ary, commandLists);
}
