#include "MyPrimitiveDrawer.h"
#include <string>
#include "Result.h"

MyPrimitiveDrawer::MyPrimitiveDrawer(ID3D12Device* device) :
	vertices(new Vec3[4]), VERT_NUM(4), blendDesc(pipelineDesc.BlendState.RenderTarget[0])
{
	//// 頂点データ
	//vertices[0] = { -0.5f, -0.5f, 0.0f }; // 左下
	//vertices[1] = { -0.5f, +0.5f, 0.0f }; // 左上
	//vertices[2] = { +0.5f, -0.5f, 0.0f }; // 右下
	//vertices[3] = { +0.5f, +0.5f, 0.0f }; // 右上

	//// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//sizeVB = static_cast<UINT>(sizeof(Vec3) * VERT_NUM);

	//// 頂点バッファ設定
	//heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	//// リソース設定
	//resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	//resDesc.Height = 1;
	//resDesc.DepthOrArraySize = 1;
	//resDesc.MipLevels = 1;
	//resDesc.SampleDesc.Count = 1;
	//resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//// 頂点バッファの生成
	//Result(device->CreateCommittedResource(
	//	&heapProp, // ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&resDesc, // リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff)));

	//// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	//Result(vertBuff->Map(0, nullptr, (void**)&vertMap));
	//// 全頂点に対して
	//for (int i = 0; i < VERT_NUM; i++)
	//{
	//	vertMap[i] = vertices[i]; // 座標をコピー
	//}
	//// 繋がりを解除
	//vertBuff->Unmap(0, nullptr);

	//// 頂点バッファビューの作成
	//// GPU仮想アドレス
	//vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//// 頂点バッファのサイズ
	//vbView.SizeInBytes = sizeVB;
	//// 頂点1つ分のデータサイズ
	//vbView.StrideInBytes = sizeof(Vec3);

	//// 頂点シェーダの読み込みとコンパイル
	//CheckError(!Result(D3DCompileFromFile(
	//	L"BasicVS.hlsl", // シェーダファイル名
	//	nullptr,
	//	D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
	//	"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
	//	D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
	//	0,
	//	&vsBlob, &errorBlob)));

	//// ピクセルシェーダの読み込みとコンパイル
	//CheckError(!Result(D3DCompileFromFile(
	//	L"BasicPS.hlsl", // シェーダファイル名
	//	nullptr,
	//	D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
	//	"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
	//	D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
	//	0,
	//	&psBlob, &errorBlob)));

	//inputLayout[0] =
	//{

	//	//{
	//	//	 セマンティック名,
	//	//	 複数同じセマンティック名がある場合に使うインデックス (0 でいい),
	//	//	 要素数とビット数を返す (float XYZ(3つ) -> R32G32B32_FLOAT),
	//	//	 *入力スロットインデックス (0 でいい),
	//	//	 データのオフセット値 (D3D12_APPEND_ALIGNED_ELEMENT だと自動),
	//	//	 *入力データ種別 (標準は D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA),
	//	//	 *一度に描画するインスタンス数 (0 でいい)
	//	//}

	//	//{
	//		"POSITION",
	//		0,
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		0,
	//		D3D12_APPEND_ALIGNED_ELEMENT,
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	//		0
	//	//}, // (1行で書いたほうが見やすい)

	//	// 座標以外を渡す場合はさらに続ける
	//};

	//// グラフィックスパイプライン設定
	//// シェーダーの設定
	//pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	//pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	//pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	//pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//// サンプルマスクの設定
	//pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	//// ラスタライザの設定
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	////pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ワイヤーフレーム
	//pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	//// ブレンドステート
	//blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	//blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画

	//blendDesc.BlendEnable = true;                // ブレンドを有効にする
	//blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算
	//blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;   // ソースの値を100%使う
	//blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; // デストの値を  0%使う

	//// 加算合成
	//blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	//blendDesc.SrcBlend = D3D12_BLEND_ONE;   // ソースの値を100%使う
	//blendDesc.DestBlend = D3D12_BLEND_ONE;  // デストの値を100%使う

	//// 減算合成
	//blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; // 減算(デスト - ソース)
	//blendDesc.SrcBlend = D3D12_BLEND_ONE;   // ソースの値を100%使う
	//blendDesc.DestBlend = D3D12_BLEND_ONE;  // デストの値を100%使う

	//// 色反転
	//blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	//blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; // 1.0f - デストカラーの値
	//blendDesc.DestBlend = D3D12_BLEND_ZERO;          // デストの値を  0%使う

	//// 半透明合成
	//blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	//blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;      // ソースのアルファ値
	//blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 1.0f - ソースのアルファ値

	//// 頂点レイアウトの設定
	//pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	//pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	//// 図形の形状設定
	//pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//// その他の設定
	//pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	//pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	//pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング
	//// ルートパラメータの設定
	//D3D12_ROOT_PARAMETER rootParam = {};
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // 定数バッファビュー
	//rootParam.Descriptor.ShaderRegister = 0;                  // 定数バッファ番号
	//rootParam.Descriptor.RegisterSpace = 0;                   // デフォルト値
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダから見える


	//// ルートシグネチャ
	//ID3D12RootSignature* rootSignature;
	//// ルートシグネチャの設定
	//D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	//rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = &rootParam; // ルートパラメータの先頭アドレス
	//rootSignatureDesc.NumParameters = 1;        // ルートパラメータ数

	//// ルートシグネチャのシリアライズ
	//ID3DBlob* rootSigBlob = nullptr;
	//Result(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
	//	&rootSigBlob, &errorBlob));
	//Result(dx->myDvc.Device()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
	//	IID_PPV_ARGS(&rootSignature)));
	//rootSigBlob->Release();
	//// パイプラインにルートシグネチャをセット
	//pipelineDesc.pRootSignature = rootSignature;

	//// パイプランステートの生成
	//ID3D12PipelineState* pipelineState = nullptr;
	//Result(dx.myDvc.Device()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	//// 定数バッファ用データ構造体 (マテリアル)
	//struct ConstBufferDataMaterial
	//{
	//	XMFLOAT4 color;
	//};

	//// ヒープ設定
	//D3D12_HEAP_PROPERTIES cbHeapProp{};
	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//// リソース設定
	//D3D12_RESOURCE_DESC cbResourceDesc{};
	//cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) * 0xff) & ~0xff;
	//cbResourceDesc.Height = 1;
	//cbResourceDesc.DepthOrArraySize = 1;
	//cbResourceDesc.MipLevels = 1;
	//cbResourceDesc.SampleDesc.Count = 1;
	//cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//ID3D12Resource* constBuffMaterial = nullptr;
	//// 定数バッファの生成
	//Result(dx.myDvc.Device()->CreateCommittedResource(
	//	&cbHeapProp, // ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc, // リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffMaterial)));

	//// 定数バッファのマッピング
	//ConstBufferDataMaterial* constMapMaterial = nullptr;
	//Result(constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial));
	//// 値を書きこむと自動的に転送される
	//constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);
	////float r = 1.0f;
	////float g = 0.0f;

	//uint16_t indices[] =
	//{
	//	0, 1, 2,
	//	1, 2, 3,
	//};

	//// インデックスデータ全体のサイズ
	//UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//// リソース設定
	//resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
	//resDesc.Height = 1;
	//resDesc.DepthOrArraySize = 1;
	//resDesc.MipLevels = 1;
	//resDesc.SampleDesc.Count = 1;
	//resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//// インデックスバッファの生成
	//ID3D12Resource* indexBuff = nullptr;
	//Result(dx.myDvc.Device()->CreateCommittedResource(
	//	&heapProp, // ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&resDesc, // リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&indexBuff)));

	//// インデックスバッファをマッピング
	//uint16_t* indexMap = nullptr;
	//Result(indexBuff->Map(0, nullptr, (void**)&indexMap));
	//// 全インデックスに対して
	//for (int i = 0; i < _countof(indices); i++)
	//{
	//	indexMap[i] = indices[i];
	//}
	//// マッピング解除
	//indexBuff->Unmap(0, nullptr);

	//// インデックスバッファビューの作成
	//D3D12_INDEX_BUFFER_VIEW ibView{};
	//ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	//ibView.Format = DXGI_FORMAT_R16_UINT;
	//ibView.SizeInBytes = sizeIB;

}

//MyPrimitiveDrawer::MyPrimitiveDrawer(const int vertNum) :
//	vertices(new Vec3[vertNum]), VERT_NUM(vertNum)
//{
//}

MyPrimitiveDrawer::~MyPrimitiveDrawer()
{
	delete[] vertices;
	vertices = 0;
}

void MyPrimitiveDrawer::SetVertex(const int vertNum, const Vec3& pos)
{
	if (VERT_NUM <= vertNum) return;
	vertices[vertNum] = pos;
}

void MyPrimitiveDrawer::CheckError(const bool b)
{
	if (b) // エラーなら
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		Assert(0);
	}
}
