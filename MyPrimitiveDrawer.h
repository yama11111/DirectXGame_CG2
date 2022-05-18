//#pragma once
//#include "Vec3.h"
//#include <d3d12.h>
//#include <d3dcompiler.h>
//
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//
//class MyPrimitiveDrawer
//{
//private:
//	Vec3* vertices = nullptr;
//	const int VERT_NUM;
//	UINT sizeVB;
//	// ���_�o�b�t�@�ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{};
//		// ���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC resDesc{};
//	// ���_�o�b�t�@�̐���
//	ID3D12Resource* vertBuff = nullptr;
//
//	Vec3* vertMap = nullptr;
//
//	// ���_�o�b�t�@�r���[�̍쐬
//	D3D12_VERTEX_BUFFER_VIEW vbView{};
//
//	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
//	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
//	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
//
//	// ���_���C�A�E�g
//	D3D12_INPUT_ELEMENT_DESC inputLayout[1];
//	const int LAYOUT_NUM = 1;
//
//	// �O���t�B�b�N�X�p�C�v���C���ݒ�
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
//
//	// �u�����h�X�e�[�g
//	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc;
//
//public:
//	MyPrimitiveDrawer(ID3D12Device* device);
//	//MyPrimitiveDrawer(const int vertNum);
//	~MyPrimitiveDrawer();
//	void SetVertex(const int vertNum, const Vec3& pos);
//private:
//	void CheckError(const bool b);
//};
//
