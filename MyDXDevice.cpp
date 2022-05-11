#include "MyDXDevice.h"
#include "Result.h"

void MyDXDevice::Init(IDXGIAdapter4* tmpAdapter)
{
	if (tmpAdapter == nullptr) return;
	for (size_t i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイス生成
		if (Result(D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device))))
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
}

ID3D12Device* MyDXDevice::Device()
{
	return device;
}
