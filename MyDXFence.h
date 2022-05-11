#pragma once
#include <d3d12.h>

#pragma comment(lib, "d3d12.lib")

class MyDXFence
{
private:
	// ÉtÉFÉìÉXÇÃê∂ê¨
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
public:
	void Init(ID3D12Device* device);
	ID3D12Fence* Fence();
	UINT64 Value();
	void WaitCommandComplete(ID3D12CommandQueue* commandQueue);
};

