#include "MyDXCommandList.h"
#include "Result.h"

void MyDXCommandList::Init(ID3D12Device* device)
{
	if (device == nullptr) return;
	// コマンドアロケータを生成
	Result(device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator)));
	// コマンドリストを生成
	Result(device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator, nullptr,
		IID_PPV_ARGS(&commandList)));
}

ID3D12CommandAllocator* MyDXCommandList::Allocator()
{
	return cmdAllocator;
}

ID3D12GraphicsCommandList* MyDXCommandList::CommandList()
{
	return commandList;
}

void MyDXCommandList::ClearScreen(const D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle,				D3D12_CPU_DESCRIPTOR_HANDLE &dsvHandle, const ClearColor& color)
{
	FLOAT clearColor[] = { color.r,color.g, color.b, color.a };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MyDXCommandList::CloseCommand()
{
	// 命令のクローズ
	Result(commandList->Close());
}

void MyDXCommandList::ClearQueue()
{
	Result(cmdAllocator->Reset());
}

void MyDXCommandList::PreStore()
{
	Result(commandList->Reset(cmdAllocator, nullptr));
}
