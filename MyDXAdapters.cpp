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
	// DXGI�t�@�N�g���[����
	Result(CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory)));

	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[���
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// ���I�z��ɒǉ�
		adapters.push_back(tmpAdapter);
	}
}

void MyDXAdapters::Select()
{
	// �Ó��ȃA�_�v�^��I��
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏��擾
		adapters[i]->GetDesc3(&adapterDesc);
		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}
}
