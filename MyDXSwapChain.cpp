#include "MyDXSwapChain.h"
#include "Result.h"

void MyDXSwapChain::Init(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory, const HWND& hwnd)
{
	Set();
	Create(commandQueue, dxgiFactory, hwnd);
}

IDXGISwapChain4* MyDXSwapChain::SwapChain()
{
	return swapChain;
}

DXGI_SWAP_CHAIN_DESC1 MyDXSwapChain::SwapChainDesc()
{
	return swapChainDesc;
}

void MyDXSwapChain::FlipBuffer()
{
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	Result(swapChain->Present(1, 0));
}

void MyDXSwapChain::Set()
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

void MyDXSwapChain::Create(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory, const HWND& hwnd)
{
	// �X���b�v�`�F�[������
	Result(dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, hwnd, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain));
}
