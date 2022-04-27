#include "MyDXSwapChain.h"

void MyDXSwapChain::Init()
{
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		  // �F���̏���
	swapChainDesc.SampleDesc.Count = 1;						  // �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;		  // �o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2;							  // �o�b�t�@����2�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
}

void MyDXSwapChain::Create(ID3D12CommandQueue*& commandQueue, IDXGIFactory7*& dxgiFactory, HWND& hwnd)
{
	HRESULT result;
	// �X���b�v�`�F�[������
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, hwnd, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain);
	assert(SUCCEEDED(result));
}
