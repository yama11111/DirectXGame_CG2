#pragma once
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

class MyDXCommandQueue
{
private:
	ID3D12CommandQueue* commandQueue = nullptr;
	//コマンドキュー設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
public:
	void Init(ID3D12Device* device);
	ID3D12CommandQueue* CommandQueue();
	void ExeCommmand(const int ary, ID3D12CommandList* commandLists);
};

