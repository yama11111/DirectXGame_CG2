#include "MyDXAdapters.h"
#include "Result.h"

void MyDXAdapters::Init()
{
	List();
	Select();
}

IDXGIAdapter4* MyDXAdapters::Template()
{
	return tmpAdapter;
}

IDXGIFactory7* MyDXAdapters::Factory()
{
	return dxgiFactory;
}

void MyDXAdapters::List()
{
	// DXGIファクトリー生成
	Result(CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory)));

	// パフォーマンスが高いものから順に、全てのアダプターを列挙
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// 動的配列に追加
		adapters.push_back(tmpAdapter);
	}
}

void MyDXAdapters::Select()
{
	// 妥当なアダプタを選別
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報取得
		adapters[i]->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}
}
