#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>

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
	void Init(IDXGIAdapter4* tmpAdapter);
	ID3D12Device* Device();
};

