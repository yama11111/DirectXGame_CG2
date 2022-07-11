#include "MyWindows.h"
#include "MyDirectX.h"
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include "MyDirectInput.h"
#include "Keys.h"
#include"MyMath.h"
#include"Result.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
// �E�B���h�E�T�C�Y 
const Vec2 WIN_SIZE(1280, 720); // (��, �c)

// �萔�o�b�t�@�p�f�[�^�\���� (�}�e���A��)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color; // �F (RGBA)
};
// �萔�o�b�t�@�p�f�[�^�\���� (3D�ϊ��s��)
struct ConstBufferDataTransform
{
	XMMATRIX mat; // 3D�ϊ��s��
};

struct Object3d
{
	// �萔�o�b�t�@(�s��p)
	ID3D12Resource* constBuffTransform;
	// �萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform;
	// �A�t�B���ϊ����
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	// ���[���h�ϊ��s��
	XMMATRIX matWorld;
	// �e�I�u�W�F�N�g�ւ̃|�C���^
	Object3d* parent = nullptr;
};

void InitializeObject3d(Object3d* object, ID3D12Device* device) 
{
		// �q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) * 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		Result(device->CreateCommittedResource(
			&cbHeapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&object->constBuffTransform)));

		// �萔�o�b�t�@�̃}�b�s���O
		Result(object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform));
}

void UpdateObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection) 
{
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(object->rotation.z);
	matRot *= XMMatrixRotationZ(object->rotation.x);
	matRot *= XMMatrixRotationZ(object->rotation.y);
	matTrans = XMMatrixTranslation(object->position.x, object->position.y, object->position.z);

	// ���[���h�s��̍���
	object->matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
	object->matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	object->matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	object->matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	if (object->parent != nullptr)
	{
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		object->matWorld *= object->parent->matWorld;
	}

	// �萔�o�b�t�@�փf�[�^�]��
	object->constMapTransform->mat = object->matWorld * matView * matProjection;
}

void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView, 
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices)
{
	// ���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vbView);

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&ibView);

	// �萔�o�b�t�@�r���[(cbv)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());
	// �`��R�}���h
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}

// �G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Windows ������
	MyWindows* win = MyWindows::GetInstance();
	win->Create(L"DirectXGame", WIN_SIZE);

	// DirectX ������
	MyDirectX* dx = MyDirectX::GetInstance();
	dx->Init(win->HandleWindow());

	// DirectInput ������
	MyDirectInput* input = MyDirectInput::GetInstance();
	input->Init(win->HandleWindowInstance(), win->HandleWindow());
	Keys* keys = Keys::GetInstance();

	InitMath();

	// ------------------------------ //

	// ------- �`�揉�������� ------- //
	// ���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv; // uv���W
	};

	// ���_�f�[�^
	Vertex vertices[] =
	{
		// �O
		{{ -5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}}, // ����
		{{ -5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}}, // ����
		{{  5.0f, -5.0f, -5.0f }, {}, {1.0f, 1.0f}}, // �E��
		{{  5.0f,  5.0f, -5.0f }, {}, {1.0f, 0.0f}}, // �E��

		// ��
		{{ -5.0f, -5.0f,  5.0f }, {}, {0.0f, 1.0f}}, // ����
		{{ -5.0f,  5.0f,  5.0f }, {}, {0.0f, 0.0f}}, // ����
		{{  5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}}, // �E��
		{{  5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}}, // �E��

		// ��
		{{ -5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}}, // ����
		{{ -5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}}, // ����
		{{ -5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}}, // �E��
		{{ -5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}}, // �E��

		// �E
		{{  5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}}, // ����
		{{  5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}}, // ����
		{{  5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}}, // �E��
		{{  5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}}, // �E��

		// ��
		{{ -5.0f, -5.0f,  5.0f }, {}, {0.0f, 1.0f}}, // ����
		{{ -5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f}}, // ����
		{{  5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}}, // �E��
		{{  5.0f, -5.0f, -5.0f }, {}, {1.0f, 0.0f}}, // �E��

		// ��
		{{ -5.0f,  5.0f,  5.0f }, {}, {0.0f, 1.0f}}, // ����
		{{ -5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}}, // ����
		{{  5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f}}, // �E��
		{{  5.0f,  5.0f, -5.0f }, {}, {1.0f, 0.0f}}, // �E��
	};

	// �C���f�b�N�X�f�[�^
	uint16_t indices[] =
	{
		// �O
		0, 1, 2, // �O�p�`1��
		2, 1, 3, // �O�p�`2��

		// ��
		6, 7, 4, // �O�p�`1��
		4, 7, 5, // �O�p�`2��

		// ��
		8, 9, 10, // �O�p�`1��
		10, 9, 11, // �O�p�`2��

		// �E
		14, 15, 12, // �O�p�`1��
		12, 15, 13, // �O�p�`2��

		// ��
		16, 17, 18, // �O�p�`1��
		18, 17, 19, // �O�p�`2��

		//// ��
		22, 23, 20, // �O�p�`1��
		20, 23, 21, // �O�p�`2��
	};

	//// �C���f�b�N�X�f�[�^
	//uint16_t indices[] =
	//{
	//	// �O
	//	0, 1, 2, // �O�p�`1��
	//	2, 1, 3, // �O�p�`2��

	//	// ��
	//	5, 4, 6, // �O�p�`1��
	//	5, 6, 7, // �O�p�`2��

	//	// ��
	//	8, 9, 10, // �O�p�`1��
	//	10, 9, 11, // �O�p�`2��

	//	// �E
	//	13, 12, 14, // �O�p�`1��
	//	13, 14, 15, // �O�p�`2��

	//	// ��
	//	16, 17, 18, // �O�p�`1��
	//	18, 17, 19, // �O�p�`2��

	//	//// ��
	//	21, 20, 22, // �O�p�`1��
	//	21, 22, 23, // �O�p�`2��
	//};


	for (int i = 0; i < 36 / 3; i++)
	{// �O�p�`1���ƂɌv�Z���Ă���
		// �O�p�`�̃C���f�b�N�X�����o���āA��ʓI�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		// �O�p�`���\�����钸�_���W�x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		// p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z (�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		// �O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		// ���K�� (������1�ɂ���)
		normal = XMVector3Normalize(normal);
		// ���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
		int a = 0;
	}


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
	Result(dx->myDvc.Device()->CreateCommittedResource(
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
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},

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
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �J�����O���Ȃ�
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

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1; // 1�x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0; // �e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// �萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // �萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;                  // �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;                   // �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // �S�ẴV�F�[�_���猩����
	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange; // �萔�o�b�t�@�ԍ�
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;              // �f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		// �S�ẴV�F�[�_���猩����
	// �萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // �萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;                  // �萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;                   // �f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // �S�ẴV�F�[�_���猩����

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // ���܂�Ԃ�   (�^�C�����O)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // �c�܂�Ԃ�   (�^�C�����O)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // ���s�܂�Ԃ� (�^�C�����O)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; // �{�[�_�[�̎��͍�
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;   // �S�ă��j�A���
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX; // �~�j�}�b�v�ő�l
	sampleDesc.MinLOD = 0.0f;              // �~�j�}�b�v�ŏ��l
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // �s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);// ���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	Result(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob));
	Result(dx->myDvc.Device()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)));
	rootSigBlob->Release();
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true; // �[�x�e�X�g
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // ��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�t�H�[�}�b�g


	// �p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;
	Result(dx->myDvc.Device()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));



	ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	{
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

		// �萔�o�b�t�@�̐���
		Result(dx->myDvc.Device()->CreateCommittedResource(
			&cbHeapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial)));

		// �萔�o�b�t�@�̃}�b�s���O
		Result(constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial));
		// �l���������ނƎ����I�ɓ]�������
		constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);
		//float r = 1.0f;
		//float g = 0.0f;
	}

	//ID3D12Resource* constBuffTransform0 = nullptr;
	//ConstBufferDataTransform* constMapTransform0 = nullptr;

	//ID3D12Resource* constBuffTransform1 = nullptr;
	//ConstBufferDataTransform* constMapTransform1 = nullptr;

	//{
	//	// �q�[�v�ݒ�
	//	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//	// ���\�[�X�ݒ�
	//	D3D12_RESOURCE_DESC cbResourceDesc{};
	//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) * 0xff) & ~0xff;
	//	cbResourceDesc.Height = 1;
	//	cbResourceDesc.DepthOrArraySize = 1;
	//	cbResourceDesc.MipLevels = 1;
	//	cbResourceDesc.SampleDesc.Count = 1;
	//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	// �萔�o�b�t�@�̐���
	//	Result(dx->myDvc.Device()->CreateCommittedResource(
	//		&cbHeapProp, // �q�[�v�ݒ�
	//		D3D12_HEAP_FLAG_NONE,
	//		&cbResourceDesc, // ���\�[�X�ݒ�
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&constBuffTransform0)));

	//	// �萔�o�b�t�@�̃}�b�s���O
	//	Result(constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0));

	//	// �萔�o�b�t�@�̐���
	//	Result(dx->myDvc.Device()->CreateCommittedResource(
	//		&cbHeapProp, // �q�[�v�ݒ�
	//		D3D12_HEAP_FLAG_NONE,
	//		&cbResourceDesc, // ���\�[�X�ݒ�
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&constBuffTransform1)));

	//	// �萔�o�b�t�@�̃}�b�s���O
	//	Result(constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1));
	//}

	//constMapTransform0->mat = XMMatrixIdentity();
	//constMapTransform0->mat.r[0].m128_f32[0] = 2.0f / WIN_SIZE.x;
	//constMapTransform0->mat.r[1].m128_f32[1] = -2.0f / WIN_SIZE.y;
	//constMapTransform0->mat.r[3].m128_f32[0] = -1.0f;
	//constMapTransform0->mat.r[3].m128_f32[1] = 1.0f;

	//// ���s�ˉe�s��̌v�Z
	//constMapTransform0->mat = XMMatrixOrthographicOffCenterLH(
	//	0.0f, WIN_SIZE.x, // ���[, �E�[
	//	WIN_SIZE.y, 0.0f, // ���[, ��[
	//	0.0f, 1.0f        // �O�[, ���[
	//);
	
	// 3D�I�u�W�F�N�g�̐�
	const size_t kObjectCount = 11;
	// 3D���I�u�W�F�N�g�̔z��
	Object3d object3ds[kObjectCount];

	for (int i = 0; i < _countof(object3ds); i++)
	{
		// ������
		InitializeObject3d(&object3ds[i], dx->myDvc.Device());

		//// ��������́��e�q�\���̃T���v��
		//// �擪�ȊO�Ȃ�
		//if (i > 0)
		//{
		//	// �ЂƂO�̃I�u�W�F�N�g��e�I�u�W�F�N�g�Ƃ���
		//	object3ds[i].parent = &object3ds[i - 1];
		//	// �e�I�u�W�F�N�g��9���̑傫��
		//	object3ds[i].scale = { 0.9f,0.9f,0.9f };
		//	// �e�I�u�W�F�N�g�ɑ΂���Z���܂���30�x��]
		//	object3ds[i].rotation = { 0.0f,0.0f,XMConvertToRadians(30.0f) };
		//	// �e�I�u�W�F�N�g�ɑ΂���Z������-8.0���炷
		//	object3ds[i].position = { 0.0f,0.0f,-8.0f };
		//}
	}

	object3ds[1].parent = &object3ds[0]; // ��
	object3ds[1].scale = { 0.75f,0.75f,0.75f };
	object3ds[1].position = { 0.0f,7.5f,0.0f };

	object3ds[2].parent = &object3ds[0]; // �����g
	object3ds[2].scale = { 1.0,1.0f,1.0f };
	object3ds[2].position = { 0.0f,-10.0f,0.0f };

	object3ds[3].parent = &object3ds[0]; // �r (�E��)
	object3ds[3].scale = { 0.5f,1.0f,0.5f };
	object3ds[3].position = { 7.5f,0.0f,0.0f };
	object3ds[4].parent = &object3ds[3]; // �r (�E��)
	object3ds[4].scale = { 0.5f,1.0f,0.5f };
	object3ds[4].position = { 0.0f,-10.0f,0.0f };

	object3ds[5].parent = &object3ds[0]; // �r (����)
	object3ds[5].scale = { 0.5f,1.0f,0.5f };
	object3ds[5].position = { -7.5f,0.0f,0.0f };
	object3ds[6].parent = &object3ds[5]; // �r (����)
	object3ds[6].scale = { 0.5f,1.0f,0.5f };
	object3ds[6].position = { 0.0f,-10.0f,0.0f };

	object3ds[7].parent = &object3ds[2]; // �� (�E��)
	object3ds[7].scale = { 0.5f,1.0f,0.5f };
	object3ds[7].position = { 5.0f,-10.0f,0.0f };
	object3ds[8].parent = &object3ds[7]; // �� (�E��)
	object3ds[8].scale = { 0.5f,1.0f,0.5f };
	object3ds[8].position = { 0.0f,-10.0f,0.0f };

	object3ds[9].parent = &object3ds[2]; // �� (����)
	object3ds[9].scale = { 0.5f,1.0f,0.5f };
	object3ds[9].position = { -5.0f,-10.0f,0.0f };
	object3ds[10].parent = &object3ds[9]; // �� (����)
	object3ds[10].scale = { 0.5f,1.0f,0.5f };
	object3ds[10].position = { 0.0f,-10.0f,0.0f };

	
	// �������e�s��̌v�Z
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),      // �㉺��p45�x
		(float)WIN_SIZE.x / WIN_SIZE.y, // �A�X�y�N�g�� (��ʉ���/��ʏc��)
		0.1f, 1000.0f                   // �O�[, ���[
	);

	// �r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100); // ���_���W
	XMFLOAT3 target(0, 0, 0); // �����_���W
	XMFLOAT3 up(0, 1, 0);     // ������x�N�g��
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//// ���[���h�ϊ��s��
	//XMMATRIX matWorld;
	//matWorld = {
	//	1,0,0,0,
	//	0,1,0,0,
	//	0,0,1,0,
	//	0,0,0,1,
	//};

	//XMMATRIX matScale; // �X�P�[�����O�s��
	//matScale = XMMatrixScaling(1.0f, 0.5f, 1.0f);
	//matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f

	//XMMATRIX matRot; // ��]�s��
	//matRot = XMMatrixIdentity();
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f)); // Z������ɉ�]
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(15.0f)); // X������ɉ�]
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(30.0f)); // Y������ɉ�]
	//matWorld *= matRot; // ���[���h�ϊ��s��ɉ�]�𔽉f

	//XMMATRIX matTrans; // ���s�ړ��s��
	//matTrans = XMMatrixTranslation(-50.0f, 0, 0);
	//matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	////////

	// �萔�o�b�t�@�ɓ]��
	//constMapTransform0->mat = matProjection;
	//constMapTransform0->mat = matView * matProjection;
	//constMapTransform0->mat = matWorld * matView * matProjection;

	float angle = 0.0f; // �J�����̉�]�p

	XMFLOAT3 scale = { 1,1,1 }; // �X�P�[�����O�{��
	XMFLOAT3 rotation = { 0,0,0 }; // ��]�p
	XMFLOAT3 position = { 0,0,0 }; // ���W

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WIN_SIZE.x; // �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = WIN_SIZE.y; // �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // �f�v�X�X�e���V��

	// �[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	// �[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // �[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g

	// ���\�[�X����
	ID3D12Resource* depthBuff = nullptr;
	Result(dx->myDvc.Device()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	));

	// �[�x�r���[�p�f�X�N���v�^�[�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	Result(dx->myDvc.Device()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap)));

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx->myDvc.Device()->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());



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
	Result(dx->myDvc.Device()->CreateCommittedResource(
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

	// �������s�N�Z����
	const size_t textureWidth = 256;
	// �c�����s�N�Z����
	const size_t textureHeight = 256;
	// �z��̗v�f��
	const size_t imageDataCount = textureWidth * textureHeight;
	// �摜�C���[�W�f�[�^�z��
	XMFLOAT4* imageData = new XMFLOAT4[imageDataCount]; // ���K����ŊJ������

	// �S�s�N�Z���̐F��������
	//for (size_t i = 0; i < imageDataCount; i++)
	//{
	//	imageData[i].x = 1.0f; // R
	//	imageData[i].y = 0.0f; // G
	//	imageData[i].z = 0.0f; // B
	//	imageData[i].w = 1.0f; // A
	//}

	bool trans = true;
	for (size_t i = 0; i < imageDataCount; i++)
	{
		imageData[i].x = GetRand(0.0f, 1.0f); // R
		imageData[i].y = GetRand(0.0f, 1.0f); // G
		imageData[i].z = GetRand(0.0f, 1.0f); // B
		if (i % 10 == 0) trans = !trans;
		imageData[i].w = trans; // A
	}

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// WIC�e�N�X�`���̃��[�h
	Result(LoadFromWICFile(L"Resources/mario.jpg", WIC_FLAGS_NONE, &metadata, scratchImg));

	ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	if (Result(GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain)))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`���� SRGB �Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	// WIC�e�N�X�`���̃��[�h
	Result(LoadFromWICFile(L"Resources/reimu.png", WIC_FLAGS_NONE, &metadata2, scratchImg2));

	ScratchImage mipChain2{};
	// �~�b�v�}�b�v����
	if (Result(GenerateMipMaps(scratchImg2.GetImages(), scratchImg2.GetImageCount(), scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain2)))
	{
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`���� SRGB �Ƃ��Ĉ���
	metadata2.format = MakeSRGB(metadata2.format);

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// ���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC textureResourceDesc{};
	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//textureResourceDesc.Width = textureWidth;   // ��
	//textureResourceDesc.Height = textureHeight; // ����
	//textureResourceDesc.DepthOrArraySize = 1;
	//textureResourceDesc.MipLevels = 1;
	//textureResourceDesc.SampleDesc.Count = 1;
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;   // ��
	textureResourceDesc.Height = (UINT)metadata.height; // ����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff = nullptr;
	Result(dx->myDvc.Device()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)));

	D3D12_RESOURCE_DESC textureResourceDesc2{};
	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width;   // ��
	textureResourceDesc2.Height = (UINT)metadata2.height; // ����
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff2 = nullptr;
	Result(dx->myDvc.Device()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2)));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//Result(texBuff->WriteToSubresource(
	//	0,
	//	nullptr, // �S�̈�փR�s�[
	//	imageData, // ���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * textureWidth, // 1���C���T�C�Y
	//	sizeof(XMFLOAT4) * imageDataCount // �S�T�C�Y
	//));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// �~�b�v�}�b�v���w�肵�ăC���[�W�擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		Result(texBuff->WriteToSubresource(
			(UINT)i,
			nullptr, // �S�̈�փR�s�[
			img->pixels, // ���f�[�^�A�h���X
			(UINT)img->rowPitch, // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		));
	}

	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		// �~�b�v�}�b�v���w�肵�ăC���[�W�擾
		const Image* img = scratchImg2.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		Result(texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr, // �S�̈�փR�s�[
			img->pixels, // ���f�[�^�A�h���X
			(UINT)img->rowPitch, // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		));
	}

	// �f�[�^�J��
	delete[] imageData;

	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// �ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;
	Result(dx->myDvc.Device()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)));

	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// �V�F�[�_���\�[�X�r���[�ݒ�
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  // RGBA float
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D�e�N�X�`��
	//srvDesc.Texture2D.MipLevels = 1;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx->myDvc.Device()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	// ��n���h����i�߂�
	UINT incrementSize = dx->myDvc.Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incrementSize;

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = textureResourceDesc2.Format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D�e�N�X�`��
	srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx->myDvc.Device()->CreateShaderResourceView(texBuff2, &srvDesc2, srvHandle);

	// ------------------------------ //

	bool flag = false;

	// �Q�[�����[�v
	while (true)
	{
		// �~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (win->CheckMessage()) break;

		// ----- DirectX ���t���[������ ----- //

		// -------------------- Update -------------------- //
		keys->Update();

		if (keys->IsDown(DIK_D) || keys->IsDown(DIK_A))
		{
			if (keys->IsDown(DIK_D)) angle += XMConvertToRadians(1.0f);
			else if (keys->IsDown(DIK_A)) angle -= XMConvertToRadians(1.0f);

			// angle���W�A������y����] (���a-100)
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}

		if (keys->IsDown(DIK_UP) || keys->IsDown(DIK_DOWN) || keys->IsDown(DIK_RIGHT) || keys->IsDown(DIK_LEFT))
		{
			// ���W�ړ� (Z���W)
			if (keys->IsDown(DIK_UP))	 object3ds[0].position.y += 1.0f;
			if (keys->IsDown(DIK_DOWN))  object3ds[0].position.y -= 1.0f;
			if (keys->IsDown(DIK_RIGHT)) object3ds[0].position.x += 1.0f;
			if (keys->IsDown(DIK_LEFT))  object3ds[0].position.x -= 1.0f;
		}

		if (keys->IsTrigger(DIK_SPACE))
		{
			if (flag) flag = false;
			else flag = true;
		}

		//// ���[���h�ϊ��s��

		//XMMATRIX matWorld;
		//matWorld = {
		//	1,0,0,0,
		//	0,1,0,0,
		//	0,0,1,0,
		//	0,0,0,1,
		//};

		//XMMATRIX matScale; // �X�P�[�����O�s��
		//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		//XMMATRIX matRot; // ��]�s��
		//matRot = XMMatrixIdentity();
		//matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z)); // Z������ɉ�]
		//matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.x)); // X������ɉ�]
		//matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.y)); // Y������ɉ�]

		//XMMATRIX matTrans; // ���s�ړ��s��
		//matTrans = XMMatrixTranslation(position.x, position.y, position.z);
		//matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
		//matWorld *= matRot; // ���[���h�ϊ��s��ɉ�]�𔽉f
		//matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

		//// �萔�o�b�t�@�ɓ]��
		//constMapTransform0->mat = matWorld * matView * matProjection;

		//XMMATRIX matWorld1;
		//matWorld1 = XMMatrixIdentity();

		//XMMATRIX matScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f); // �X�P�[�����O�s��
		//XMMATRIX matRot1 = XMMatrixRotationY(XM_PI / 4.0f); // ��]�s��
		//XMMATRIX matTrans1 = XMMatrixTranslation(-20.0f, 0, 0); // ���s�ړ��s��
		//matWorld1 = matScale1 * matRot1 * matTrans1;

		//// �萔�o�b�t�@�ɓ]��
		//constMapTransform1->mat = matWorld1 * matView * matProjection;

		for (int i = 0; i < _countof(object3ds); i++)
		{
			UpdateObject3d(&object3ds[i], matView, matProjection);
		}


		// ------------------------------------------------ //

		// �`�揀��
		dx->PreDraw(dsvHeap);
		// --------------------- Draw --------------------- //

		// �r���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport{};
		viewport.Width = WIN_SIZE.x; // ����
		viewport.Height = WIN_SIZE.y; // �c��
		viewport.TopLeftX = 0; // ���� X
		viewport.TopLeftY = 0; // ���� Y
		viewport.MinDepth = 0.0f; // �k���[�x (0 �ł���)
		viewport.MaxDepth = 1.0f; // �g��[�x (1 �ł���)
		// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		dx->myCmdList.CommandList()->RSSetViewports(1, &viewport);

		// �V�U�[��`
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // �؂蔲�����W ��
		scissorRect.right = scissorRect.left + WIN_SIZE.x; // �؂蔲�����W �E
		scissorRect.top = 0; // �؂蔲�����W ��
		scissorRect.bottom = scissorRect.top + WIN_SIZE.y; // �؂蔲�����W ��
		// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		dx->myCmdList.CommandList()->RSSetScissorRects(1, &scissorRect);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dx->myCmdList.CommandList()->SetPipelineState(pipelineState);
		dx->myCmdList.CommandList()->SetGraphicsRootSignature(rootSignature);

		// �v���~�e�B�u�`��̐ݒ�R�}���h
		dx->myCmdList.CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		dx->myCmdList.CommandList()->IASetVertexBuffers(0, 1, &vbView);

		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		dx->myCmdList.CommandList()->IASetIndexBuffer(&ibView);

		// �萔�o�b�t�@
		dx->myCmdList.CommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		// SRV�q�[�v�̐ݒ�R�}���h
		dx->myCmdList.CommandList()->SetDescriptorHeaps(1, &srvHeap);
		// SRV�q�[�v�̐擪�n���h�����擾 (SRV���w�肵�Ă���͂�)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
		dx->myCmdList.CommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		// 2���ڂ��w�������悤�ɂ���SRV�̃n���h�������[�g�p�����[�^1�Ԃɐݒ�
		srvGpuHandle.ptr += incrementSize;
		if (flag) dx->myCmdList.CommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h (0)
		//dx->myCmdList.CommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		//// �`��R�}���h
		////dx.myCmdList.CommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
		//dx->myCmdList.CommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��

		//// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h (1)
		//dx->myCmdList.CommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		//// �`��R�}���h
		//dx->myCmdList.CommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��

		// �S�I�u�W�F�N�g�ɂ��ď���
		for (int i = 0; i < _countof(object3ds); i++)
		{
			DrawObject3d(&object3ds[i], dx->myCmdList.CommandList(), vbView, ibView, _countof(indices));
		}

		// -------------------------------------------------- //
		// �`��㏈��
		dx->PostDraw();

		// ---------------------------------- //
	}

	// �E�B���h�E�N���X��o�^����
	win->FinalProcess();

	return 0;
}