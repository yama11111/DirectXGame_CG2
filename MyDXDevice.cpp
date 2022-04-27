#include "MyDXDevice.h"
#include <cassert>

void MyDXDevice::Create(IDXGIAdapter4*& tmpAdapter)
{
	HRESULT result;
	for (size_t i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイス生成
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
}
