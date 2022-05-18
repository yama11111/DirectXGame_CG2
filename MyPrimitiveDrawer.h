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
//	// 頂点バッファ設定
//	D3D12_HEAP_PROPERTIES heapProp{};
//		// リソース設定
//	D3D12_RESOURCE_DESC resDesc{};
//	// 頂点バッファの生成
//	ID3D12Resource* vertBuff = nullptr;
//
//	Vec3* vertMap = nullptr;
//
//	// 頂点バッファビューの作成
//	D3D12_VERTEX_BUFFER_VIEW vbView{};
//
//	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
//	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
//	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
//
//	// 頂点レイアウト
//	D3D12_INPUT_ELEMENT_DESC inputLayout[1];
//	const int LAYOUT_NUM = 1;
//
//	// グラフィックスパイプライン設定
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
//
//	// ブレンドステート
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
