#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDXResourceBarrier
{
private:
	UINT bbIndex;
	D3D12_RESOURCE_BARRIER barrierDesc{};
public:
	void ChangeToDraw(ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain, 
						std::vector<ID3D12Resource*> backBuffers);
	void ChangeToDisplay(ID3D12GraphicsCommandList* commandList);
	UINT BBIndex();
};

