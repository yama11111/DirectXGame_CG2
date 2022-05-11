#include "MyDXRenderTargetView.h"

void MyDXRenderTargetView::Init(ID3D12Device* device, IDXGISwapChain4* swapChain, const DXGI_SWAP_CHAIN_DESC1& swapChainDesc)
{
	SetDescriptorHeap(swapChainDesc);
	CreateDescriptorHeap(device);
	SetBackBuffer(swapChainDesc);
	CreateRenderTargetView(device, swapChain);
}

std::vector<ID3D12Resource*> MyDXRenderTargetView::BackBuffers()
{
	return backBuffers;
}

D3D12_CPU_DESCRIPTOR_HANDLE MyDXRenderTargetView::Handle()
{
	return rtvHeap->GetCPUDescriptorHandleForHeapStart();
}

D3D12_DESCRIPTOR_HEAP_DESC MyDXRenderTargetView::Desc()
{
	return rtvHeapDesc;
}

void MyDXRenderTargetView::SetDescriptorHeap(const DXGI_SWAP_CHAIN_DESC1& swapChainDesc)
{
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // ���\��2��
}

void MyDXRenderTargetView::CreateDescriptorHeap(ID3D12Device* device)
{
	// �f�X�N���v�^�q�[�v����
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
}

void MyDXRenderTargetView::SetBackBuffer(const DXGI_SWAP_CHAIN_DESC1& swapChainDesc)
{
	backBuffers.resize(swapChainDesc.BufferCount);
}

void MyDXRenderTargetView::CreateRenderTargetView(ID3D12Device* device, IDXGISwapChain4* swapChain)
{
	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@�擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h���擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// �����_�[�^�[�Q�b�g�r���[�ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[����
		device->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}
}
