#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
//#include "MyDXAdapters.h"
//#include "MyDXDevice.h"
//#include "MyDXCommandList.h"
//#include "MyDXCommandQueue.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDirectX
{
private:
	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	// �A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter4*> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter4* tmpAdapter = nullptr;
	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[4] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	//�R�}���h�L���[�ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// �X���b�v�`�F�[���ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// �f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	
	// �o�b�N�o�b�t�@
	std::vector<ID3D12Resource*> backBuffers;

public:
	MyDirectX();
	~MyDirectX();
private:
	void ListGraphicsBoardAdapters();
	void SelectGraphiscBoardAdapters();
	void CreateDevice();
	void CreateCommandList();
	void CreateCommandQueue();
	void SetSwapChain();
	void CreateSwapChain(HWND& hwnd);
	void SetDescriptorHeap();
	void CreateDescriptorHeap();
	void BackBaffer();
	void CreateRenderTargetView();
};

