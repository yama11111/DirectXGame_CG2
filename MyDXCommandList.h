#pragma once
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

class MyDXCommandList
{
private:
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
public:
	void Init(ID3D12Device* device);
	ID3D12CommandAllocator* Allocator();
	ID3D12GraphicsCommandList* CommandList();
	struct ClearColor
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;
	};
	void ClearScreen(const D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle, const ClearColor& color);
	void CloseCommand();
	void ClearQueue();
	void PreStore();
};

