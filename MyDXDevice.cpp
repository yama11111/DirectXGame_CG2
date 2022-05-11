#include "MyDXDevice.h"
#include "Result.h"

void MyDXDevice::Init(IDXGIAdapter4* tmpAdapter)
{
	if (tmpAdapter == nullptr) return;
	for (size_t i = 0; i < _countof(levels); i++)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X����
		if (Result(D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device))))
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
}

ID3D12Device* MyDXDevice::Device()
{
	return device;
}
