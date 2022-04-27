#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDXDevice
{
private:
	ID3D12Device* device = nullptr;
	// ‘Î‰žƒŒƒxƒ‹‚Ì”z—ñ
	D3D_FEATURE_LEVEL levels[4] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	D3D_FEATURE_LEVEL featureLevel;
public:
	MyDXDevice();
	~MyDXDevice();
	void Create(IDXGIAdapter4*& tmpAdapter);
};

