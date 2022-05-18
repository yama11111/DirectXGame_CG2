#include <Windows.h>
#include "MyWindowsApplication.h"
#include "MyDirectX.h"
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "MyDirectInput.h"
#include "Keys.h"
#include"Result.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

// �G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Windows ������
	MyWIndowsApplication win;
	win.CreateMyWindow(L"DirectXGame", { 1280,720 });

	// DirectX ������
	MyDirectX dx;
	dx.EnableDebugLayer();
	dx.Init(win.HandleWindow());

	// DirectInput ������
	MyDirectInput* input = MyDirectInput::GetInstance();
	input->Init(win.HandleWindowInstance(), win.HandleWindow());
	Keys* keys = Keys::GetInstance();

	// ------------------------------ //

	// ------- �`�揉�������� ------- //
	// ���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv; // uv���W
	};

	// ���_�f�[�^
	//XMFLOAT3 vertices[] =
	//{
	//	{ -0.5f, -0.5f, 0.0f }, // ����
	//	{ -0.5f, +0.5f, 0.0f }, // ����
	//	{ +0.5f, -0.5f, 0.0f }, // �E��
	//	{ +0.5f, +0.5f, 0.0f }, // �E��
	//	//{ -0.5f,  0.0f, 0.0f }, // ����
	//	//{ +0.5f,  0.0f, 0.0f }, // �E��
	//};
	Vertex vertices[] =
	{
		{{ -0.4f, -0.7f, 0.0f }, {0.0f, 1.0f}}, // ����
		{{ -0.4f, +0.7f, 0.0f }, {0.0f, 0.0f}}, // ����
		{{ +0.4f, -0.7f, 0.0f }, {1.0f, 1.0f}}, // �E��
		{{ +0.4f, +0.7f, 0.0f }, {1.0f, 0.0f}}  // �E��
		//{ -0.5f,  0.0f, 0.0f }, // ����
		//{ +0.5f,  0.0f, 0.0f }, // �E��
	};
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// ���_�o�b�t�@�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	Result(dx.myDvc.Device()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	Result(vertBuff->Map(0, nullptr, (void**)&vertMap));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	//vbView.StrideInBytes = sizeof(XMFLOAT3);
	vbView.StrideInBytes = sizeof(vertices[0]);

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	// �G���[�Ȃ�
	if (!Result(D3DCompileFromFile(
		L"BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob)))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	// �G���[�Ȃ�
	if (!Result(D3DCompileFromFile(
		L"BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob)))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{

		//{
		//	 �Z�}���e�B�b�N��,
		//	 ���������Z�}���e�B�b�N��������ꍇ�Ɏg���C���f�b�N�X (0 �ł���),
		//	 �v�f���ƃr�b�g����Ԃ� (float XYZ(3��) -> R32G32B32_FLOAT),
		//	 *���̓X���b�g�C���f�b�N�X (0 �ł���),
		//	 �f�[�^�̃I�t�Z�b�g�l (D3D12_APPEND_ALIGNED_ELEMENT ���Ǝ���),
		//	 *���̓f�[�^��� (�W���� D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA),
		//	 *��x�ɕ`�悷��C���X�^���X�� (0 �ł���)
		//}

		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}, // (1�s�ŏ������ق������₷��)

		// ���W�ȊO��n���ꍇ�͂���ɑ�����

		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ���C���[�t���[��
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; 
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��

	blendDesc.BlendEnable = true;                // �u�����h��L���ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // ���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;   // �\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; // �f�X�g�̒l��  0%�g��

	// ���Z����
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // ���Z
	blendDesc.SrcBlend = D3D12_BLEND_ONE;   // �\�[�X�̒l��100%�g��
	blendDesc.DestBlend = D3D12_BLEND_ONE;  // �f�X�g�̒l��100%�g��

	// ���Z����
	blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; // ���Z(�f�X�g - �\�[�X)
	blendDesc.SrcBlend = D3D12_BLEND_ONE;   // �\�[�X�̒l��100%�g��
	blendDesc.DestBlend = D3D12_BLEND_ONE;  // �f�X�g�̒l��100%�g��

	// �F���]
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // ���Z
	blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; // 1.0f - �f�X�g�J���[�̒l
	blendDesc.DestBlend = D3D12_BLEND_ZERO;          // �f�X�g�̒l��  0%�g��

	// ����������
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // ���Z
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;      // �\�[�X�̃A���t�@�l
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 1.0f - �\�[�X�̃A���t�@�l

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParam = {};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // �萔�o�b�t�@�r���[
	rootParam.Descriptor.ShaderRegister = 0;                  // �萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;                   // �f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // �S�ẴV�F�[�_���猩����


	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParam; // ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = 1;        // ���[�g�p�����[�^��

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	Result(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob));
	Result(dx.myDvc.Device()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)));
	rootSigBlob->Release();
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	// �p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;
	Result(dx.myDvc.Device()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	// �萔�o�b�t�@�p�f�[�^�\���� (�}�e���A��)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;
	};

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) * 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* constBuffMaterial = nullptr;
	// �萔�o�b�t�@�̐���
	Result(dx.myDvc.Device()->CreateCommittedResource(
		&cbHeapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)));

	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	Result(constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial));
	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);
	//float r = 1.0f;
	//float g = 0.0f;
	
	// �C���f�b�N�X�f�[�^
	uint16_t indices[] =
	{
		0, 1, 2, // �O�p�`1��
		1, 2, 3, // �O�p�`2��
	};

	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	// ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	Result(dx.myDvc.Device()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)));

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	Result(indexBuff->Map(0, nullptr, (void**)&indexMap));
	// �S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}
	// �}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	// ------------------------------ //

	// �Q�[�����[�v
	while (true)
	{
		// �~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (win.MessageLoop()) break;

		// ----- DirectX ���t���[������ ----- //

		// -------------------- Update -------------------- //
		keys->Update();

		
		// ------------------------------------------------ //

		// �`�揀��
		dx.PreDraw();
		// --------------------- Draw --------------------- //
		
		// �r���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport{};
		viewport.Width = win.SIZE->x; // ����
		viewport.Height = win.SIZE->y; // �c��
		viewport.TopLeftX = 0; // ���� X
		viewport.TopLeftY = 0; // ���� Y
		viewport.MinDepth = 0.0f; // �k���[�x (0 �ł���)
		viewport.MaxDepth = 1.0f; // �g��[�x (1 �ł���)
		// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		dx.myCmdList.CommandList()->RSSetViewports(1, &viewport);

		// �V�U�[��`
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // �؂蔲�����W ��
		scissorRect.right = scissorRect.left + win.SIZE->x; // �؂蔲�����W �E
		scissorRect.top = 0; // �؂蔲�����W ��
		scissorRect.bottom = scissorRect.top + win.SIZE->y; // �؂蔲�����W ��
		// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		dx.myCmdList.CommandList()->RSSetScissorRects(1, &scissorRect);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dx.myCmdList.CommandList()->SetPipelineState(pipelineState);
		dx.myCmdList.CommandList()->SetGraphicsRootSignature(rootSignature);

		// �v���~�e�B�u�`��̐ݒ�R�}���h
		dx.myCmdList.CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		dx.myCmdList.CommandList()->IASetVertexBuffers(0, 1, &vbView);

		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		dx.myCmdList.CommandList()->IASetIndexBuffer(&ibView);

		// �萔�o�b�t�@
		dx.myCmdList.CommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		//if (r > 0.0f) r -= 0.01f;
		//if (g < 1.0f) g += 0.01f;
		//constMapMaterial->color = XMFLOAT4(r, g, 0, 0.5f);

		// �`��R�}���h
		//dx.myCmdList.CommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
		dx.myCmdList.CommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
		
		// -------------------------------------------------- //
		// �`��㏈��
		dx.PostDraw();

		// ---------------------------------- //
	}

	// �E�B���h�E�N���X��o�^����
	win.FinalProcess();

	return 0;
}