#include "MyDirectX.h"
#include "Result.h"


MyDirectX* MyDirectX::GetInstance()
{
	static MyDirectX instance;
	return &instance;
}

void MyDirectX::Init(const HWND& hwnd)
{
	EnableDebugLayer();
	myAdpts.Init();
	myDvc.Init(myAdpts.Template());
	myCmdList.Init(myDvc.Device());
	myCmdQueue.Init(myDvc.Device());
	mySwapChain.Init(myCmdQueue.CommandQueue(),myAdpts.Factory(), hwnd);
	myRTV.Init(myDvc.Device(), mySwapChain.SwapChain(), mySwapChain.SwapChainDesc());
	myFence.Init(myDvc.Device());
}

void MyDirectX::PreDraw()
{
	myRscBarrier.ChangeToDraw(myCmdList.CommandList(), mySwapChain.SwapChain(), myRTV.BackBuffers());

	// 2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = myRTV.Handle();
	rtvHandle.ptr += myRscBarrier.BBIndex() * myDvc.Device()->GetDescriptorHandleIncrementSize(myRTV.Desc().Type);
	myCmdList.CommandList()->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	// 3.��ʃN���A {R, G, B, A}
	myCmdList.ClearScreen(rtvHandle, { 0.1f, 0.25f, 0.5f, 0.0f }); // ���ۂ��F
}

void MyDirectX::PostDraw()
{
	myRscBarrier.ChangeToDisplay(myCmdList.CommandList());

	// ���߂̃N���[�Y
	myCmdList.CloseCommand();
	// �R�}���h���X�g���s
	myCmdQueue.ExeCommmand(1, myCmdList.CommandList());
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	mySwapChain.FlipBuffer();

	// �R�}���h�̎��s������҂�
	myFence.WaitCommandComplete(myCmdQueue.CommandQueue());
	// �L���[���N���A
	myCmdList.ClearQueue();
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	myCmdList.PreStore();

}

void MyDirectX::EnableDebugLayer()
{
#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif
}

MyDirectX::~MyDirectX()
{
}