#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDXCommandQueue
{
private:
	ID3D12CommandQueue* commandQueue = nullptr;
public:
	MyDXCommandQueue();
	~MyDXCommandQueue();
private:
	void Create(ID3D12Device*& device);
};

