#pragma once
#include "MyDXAdapters.h"
#include "MyDXDevice.h"
#include "MyDXCommandList.h"
#include "MyDXCommandQueue.h"
#include "MyDXSwapChain.h"
#include "MyDXRenderTargetView.h"
#include "MyDXFence.h"
#include "MyDXResourceBarrier.h"

class MyDirectX
{
public:
	MyDXAdapters myAdpts;
	MyDXDevice myDvc;
	MyDXCommandList myCmdList;
	MyDXCommandQueue myCmdQueue;
	MyDXSwapChain mySwapChain;
	MyDXRenderTargetView myRTV;
	MyDXFence myFence;

	MyDXResourceBarrier myRscBarrier;

	ID3D12Debug* debugController;
public:
	MyDirectX();
	~MyDirectX();
	void Init(const HWND& hwnd);
	void PreDraw();
	void PostDraw();
	void EnableDebugLayer();
};

