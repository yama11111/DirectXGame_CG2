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
// ウィンドウサイズ 
const Vec2 WIN_SIZE(1280, 720); // (横, 縦)

// エントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Windows 初期化
	MyWindows* win = MyWindows::GetInstance();
	win->Create(L"DirectXGame", WIN_SIZE);

	// DirectX 初期化
	MyDirectX* dx = MyDirectX::GetInstance();
	dx->Init(win->HandleWindow());

	// DirectInput 初期化
	MyDirectInput* input = MyDirectInput::GetInstance();
	input->Init(win->HandleWindowInstance(), win->HandleWindow());
	Keys* keys = Keys::GetInstance();

	InitMath();

	// ------------------------------ //

	// ------- 描画初期化処理 ------- //
	// 頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv; // uv座標
	};

	// 頂点データ
	Vertex vertices[] =
	{
		// 前
		{{ -5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}}, // 左下
		{{ -5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}}, // 左上
		{{  5.0f, -5.0f, -5.0f }, {}, {1.0f, 1.0f}}, // 右下
		{{  5.0f,  5.0f, -5.0f }, {}, {1.0f, 0.0f}}, // 右上

		// 後
		{{ -5.0f, -5.0f,  5.0f }, {}, {0.0f, 1.0f}}, // 左下
		{{ -5.0f,  5.0f,  5.0f }, {}, {0.0f, 0.0f}}, // 左上
		{{  5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}}, // 右下
		{{  5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}}, // 右上

		// 左
		{{ -5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}}, // 左下
		{{ -5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}}, // 左上
		{{ -5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}}, // 右下
		{{ -5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}}, // 右上

		// 右
		{{  5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}}, // 左下
		{{  5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}}, // 左上
		{{  5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}}, // 右下
		{{  5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}}, // 右上

		// 下
		{{ -5.0f, -5.0f,  5.0f }, {}, {0.0f, 1.0f}}, // 左下
		{{ -5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f}}, // 左上
		{{  5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}}, // 右下
		{{  5.0f, -5.0f, -5.0f }, {}, {1.0f, 0.0f}}, // 右上

		// 上
		{{ -5.0f,  5.0f,  5.0f }, {}, {0.0f, 1.0f}}, // 左下
		{{ -5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}}, // 左上
		{{  5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f}}, // 右下
		{{  5.0f,  5.0f, -5.0f }, {}, {1.0f, 0.0f}}, // 右上
	};

	// インデックスデータ
	uint16_t indices[] =
	{
		// 前
		0, 1, 2, // 三角形1つ目
		2, 1, 3, // 三角形2つ目

		// 後
		6, 7, 4, // 三角形1つ目
		4, 7, 5, // 三角形2つ目

		// 左
		8, 9, 10, // 三角形1つ目
		10, 9, 11, // 三角形2つ目

		// 右
		14, 15, 12, // 三角形1つ目
		12, 15, 13, // 三角形2つ目

		// 下
		16, 17, 18, // 三角形1つ目
		18, 17, 19, // 三角形2つ目

		//// 上
		22, 23, 20, // 三角形1つ目
		20, 23, 21, // 三角形2つ目
	};

	//// インデックスデータ
	//uint16_t indices[] =
	//{
	//	// 前
	//	0, 1, 2, // 三角形1つ目
	//	2, 1, 3, // 三角形2つ目

	//	// 後
	//	5, 4, 6, // 三角形1つ目
	//	5, 6, 7, // 三角形2つ目

	//	// 左
	//	8, 9, 10, // 三角形1つ目
	//	10, 9, 11, // 三角形2つ目

	//	// 右
	//	13, 12, 14, // 三角形1つ目
	//	13, 14, 15, // 三角形2つ目

	//	// 下
	//	16, 17, 18, // 三角形1つ目
	//	18, 17, 19, // 三角形2つ目

	//	//// 上
	//	21, 20, 22, // 三角形1つ目
	//	21, 22, 23, // 三角形2つ目
	//};


	for (int i = 0; i < 36 / 3; i++)
	{// 三角形1つごとに計算していく
		// 三角形のインデックスを取り出して、一般的な変数に入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		// 三角形を構成する頂点座標ベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		// p0->p1ベクトル、p0->p2ベクトルを計算 (ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		// 外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		// 正規化 (長さを1にする)
		normal = XMVector3Normalize(normal);
		// 求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
		int a = 0;
	}


	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// 頂点バッファ設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	Result(dx->myDvc.Device()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	Result(vertBuff->Map(0, nullptr, (void**)&vertMap));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	//vbView.StrideInBytes = sizeof(XMFLOAT3);
	vbView.StrideInBytes = sizeof(vertices[0]);

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	// エラーなら
	if (!Result(D3DCompileFromFile(
		L"BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob)))
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
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	// エラーなら
	if (!Result(D3DCompileFromFile(
		L"BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob)))
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
		assert(0);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{

		//{
		//	 セマンティック名,
		//	 複数同じセマンティック名がある場合に使うインデックス (0 でいい),
		//	 要素数とビット数を返す (float XYZ(3つ) -> R32G32B32_FLOAT),
		//	 *入力スロットインデックス (0 でいい),
		//	 データのオフセット値 (D3D12_APPEND_ALIGNED_ELEMENT だと自動),
		//	 *入力データ種別 (標準は D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA),
		//	 *一度に描画するインスタンス数 (0 でいい)
		//}

		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}, // (1行で書いたほうが見やすい)

		// 座標以外を渡す場合はさらに続ける

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

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ワイヤーフレーム
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; 
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画

	blendDesc.BlendEnable = true;                // ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;   // ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; // デストの値を  0%使う

	// 加算合成
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	blendDesc.SrcBlend = D3D12_BLEND_ONE;   // ソースの値を100%使う
	blendDesc.DestBlend = D3D12_BLEND_ONE;  // デストの値を100%使う

	// 減算合成
	blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; // 減算(デスト - ソース)
	blendDesc.SrcBlend = D3D12_BLEND_ONE;   // ソースの値を100%使う
	blendDesc.DestBlend = D3D12_BLEND_ONE;  // デストの値を100%使う

	// 色反転
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; // 1.0f - デストカラーの値
	blendDesc.DestBlend = D3D12_BLEND_ZERO;          // デストの値を  0%使う

	// 半透明合成
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;      // ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 1.0f - ソースのアルファ値

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1; // 1度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0; // テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// 定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // 定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;                  // 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;                   // デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダから見える
	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange; // 定数バッファ番号
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;              // デフォルト値
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		// 全てのシェーダから見える
	// 定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // 定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;                  // 定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;                   // デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダから見える

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 横折り返し   (タイリング)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 縦折り返し   (タイリング)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 奥行折り返し (タイリング)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; // ボーダーの時は黒
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;   // 全てリニア補間
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX; // ミニマップ最大値
	sampleDesc.MinLOD = 0.0f;              // ミニマップ最小値
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーからのみ使用可能

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; // ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);// ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	Result(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob));
	Result(dx->myDvc.Device()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true; // 深度テスト
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度フォーマット


	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	Result(dx->myDvc.Device()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	// 定数バッファ用データ構造体 (マテリアル)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color; // 色 (RGBA)
	};
	// 定数バッファ用データ構造体 (3D変換行列)
	struct ConstBufferDataTransform
	{
		XMMATRIX mat; // 3D変換行列
	};

	ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		// リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) * 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		Result(dx->myDvc.Device()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial)));

		// 定数バッファのマッピング
		Result(constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial));
		// 値を書きこむと自動的に転送される
		constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);
		//float r = 1.0f;
		//float g = 0.0f;
	}

	ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		// リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) * 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		Result(dx->myDvc.Device()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform0)));

		// 定数バッファのマッピング
		Result(constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0));

		// 定数バッファの生成
		Result(dx->myDvc.Device()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform1)));

		// 定数バッファのマッピング
		Result(constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1));
	}

	constMapTransform0->mat = XMMatrixIdentity();
	constMapTransform0->mat.r[0].m128_f32[0] = 2.0f / WIN_SIZE.x;
	constMapTransform0->mat.r[1].m128_f32[1] = -2.0f / WIN_SIZE.y;
	constMapTransform0->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform0->mat.r[3].m128_f32[1] = 1.0f;

	// 平行射影行列の計算
	constMapTransform0->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_SIZE.x, // 左端, 右端
		WIN_SIZE.y, 0.0f, // 下端, 上端
		0.0f, 1.0f        // 前端, 奥端
	);
	// 投資投影行列の計算
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),      // 上下画角45度
		(float)WIN_SIZE.x / WIN_SIZE.y, // アスペクト比 (画面横幅/画面縦幅)
		0.1f, 1000.0f                   // 前端, 奥端
	);

	// ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100); // 視点座標
	XMFLOAT3 target(0, 0, 0); // 注視点座標
	XMFLOAT3 up(0, 1, 0);     // 上方向ベクトル
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//// ワールド変換行列
	//XMMATRIX matWorld;
	//matWorld = {
	//	1,0,0,0,
	//	0,1,0,0,
	//	0,0,1,0,
	//	0,0,0,1,
	//};

	//XMMATRIX matScale; // スケーリング行列
	//matScale = XMMatrixScaling(1.0f, 0.5f, 1.0f);
	//matWorld *= matScale; // ワールド行列にスケーリングを反映

	//XMMATRIX matRot; // 回転行列
	//matRot = XMMatrixIdentity();
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f)); // Z軸周りに回転
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(15.0f)); // X軸周りに回転
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(30.0f)); // Y軸周りに回転
	//matWorld *= matRot; // ワールド変換行列に回転を反映

	//XMMATRIX matTrans; // 平行移動行列
	//matTrans = XMMatrixTranslation(-50.0f, 0, 0);
	//matWorld *= matTrans; // ワールド行列に平行移動を反映

	////////

	// 定数バッファに転送
	//constMapTransform0->mat = matProjection;
	//constMapTransform0->mat = matView * matProjection;
	//constMapTransform0->mat = matWorld * matView * matProjection;

	float angle = 0.0f; // カメラの回転角

	XMFLOAT3 scale = { 1,1,1 }; // スケーリング倍率
	XMFLOAT3 rotation = { 0,0,0 }; // 回転角
	XMFLOAT3 position = { 0,0,0 }; // 座標

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WIN_SIZE.x; // レンダーターゲットに合わせる
	depthResourceDesc.Height = WIN_SIZE.y; // レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // デプスステンシル

	// 深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット

	// リソース生成
	ID3D12Resource* depthBuff = nullptr;
	Result(dx->myDvc.Device()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	));

	// 深度ビュー用デスクリプターヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	Result(dx->myDvc.Device()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap)));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx->myDvc.Device()->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());



	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	Result(dx->myDvc.Device()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)));

	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	Result(indexBuff->Map(0, nullptr, (void**)&indexMap));
	// 全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}
	// マッピング解除
	indexBuff->Unmap(0, nullptr);

	// インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	// 横方向ピクセル数
	const size_t textureWidth = 256;
	// 縦方向ピクセル数
	const size_t textureHeight = 256;
	// 配列の要素数
	const size_t imageDataCount = textureWidth * textureHeight;
	// 画像イメージデータ配列
	XMFLOAT4* imageData = new XMFLOAT4[imageDataCount]; // ※必ず後で開放する

	// 全ピクセルの色を初期化
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
	// WICテクスチャのロード
	Result(LoadFromWICFile(L"Resources/mario.jpg", WIC_FLAGS_NONE, &metadata, scratchImg));

	ScratchImage mipChain{};
	// ミップマップ生成
	if (Result(GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain)))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャを SRGB として扱う
	metadata.format = MakeSRGB(metadata.format);

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// リソース設定
	//D3D12_RESOURCE_DESC textureResourceDesc{};
	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//textureResourceDesc.Width = textureWidth;   // 幅
	//textureResourceDesc.Height = textureHeight; // 高さ
	//textureResourceDesc.DepthOrArraySize = 1;
	//textureResourceDesc.MipLevels = 1;
	//textureResourceDesc.SampleDesc.Count = 1;
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;   // 幅
	textureResourceDesc.Height = (UINT)metadata.height; // 高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	Result(dx->myDvc.Device()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)));

	// テクスチャバッファにデータ転送
	//Result(texBuff->WriteToSubresource(
	//	0,
	//	nullptr, // 全領域へコピー
	//	imageData, // 元データアドレス
	//	sizeof(XMFLOAT4) * textureWidth, // 1ラインサイズ
	//	sizeof(XMFLOAT4) * imageDataCount // 全サイズ
	//));

	// テクスチャバッファにデータ転送
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// ミップマップを指定してイメージ取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		Result(texBuff->WriteToSubresource(
			(UINT)i,
			nullptr, // 全領域へコピー
			img->pixels, // 元データアドレス
			(UINT)img->rowPitch, // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		));
	}

	// データ開放
	delete[] imageData;

	// SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// 設定をもとにSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	Result(dx->myDvc.Device()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)));

	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// シェーダリソースビュー設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  // RGBA float
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = 1;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	dx->myDvc.Device()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	// ------------------------------ //

	// ゲームループ
	while (true)
	{
		// ×ボタンで終了メッセージが来たらゲームループを抜ける
		if (win->CheckMessage()) break;

		// ----- DirectX 毎フレーム処理 ----- //

		// -------------------- Update -------------------- //
		keys->Update();

		if (keys->IsDown(DIK_D) || keys->IsDown(DIK_A))
		{
			if (keys->IsDown(DIK_D)) angle += XMConvertToRadians(1.0f);
			else if (keys->IsDown(DIK_A)) angle -= XMConvertToRadians(1.0f);

			// angleラジアンだけy軸回転 (半径-100)
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}

		if (keys->IsDown(DIK_UP) || keys->IsDown(DIK_DOWN) || keys->IsDown(DIK_RIGHT) || keys->IsDown(DIK_LEFT))
		{
			// 座標移動 (Z座標)
			if (keys->IsDown(DIK_UP))	 position.y += 1.0f;
			if (keys->IsDown(DIK_DOWN))  position.y -= 1.0f;
			if (keys->IsDown(DIK_RIGHT)) position.x += 1.0f;
			if (keys->IsDown(DIK_LEFT))  position.x -= 1.0f;
		}

		// ワールド変換行列

		XMMATRIX matWorld;
		matWorld = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1,
		};

		XMMATRIX matScale; // スケーリング行列
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		XMMATRIX matRot; // 回転行列
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z)); // Z軸周りに回転
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.x)); // X軸周りに回転
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.y)); // Y軸周りに回転

		XMMATRIX matTrans; // 平行移動行列
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
		matWorld *= matScale; // ワールド行列にスケーリングを反映
		matWorld *= matRot; // ワールド変換行列に回転を反映
		matWorld *= matTrans; // ワールド行列に平行移動を反映

		// 定数バッファに転送
		constMapTransform0->mat = matWorld * matView * matProjection;

		XMMATRIX matWorld1;
		matWorld1 = XMMatrixIdentity();

		XMMATRIX matScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f); // スケーリング行列
		XMMATRIX matRot1 = XMMatrixRotationY(XM_PI / 4.0f); // 回転行列
		XMMATRIX matTrans1 = XMMatrixTranslation(-20.0f, 0, 0); // 平行移動行列
		matWorld1 = matScale1 * matRot1 * matTrans1;

		// 定数バッファに転送
		constMapTransform1->mat = matWorld1 * matView * matProjection;

		// ------------------------------------------------ //

		// 描画準備
		dx->PreDraw(dsvHeap);
		// --------------------- Draw --------------------- //

		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		viewport.Width = WIN_SIZE.x; // 横幅
		viewport.Height = WIN_SIZE.y; // 縦幅
		viewport.TopLeftX = 0; // 左上 X
		viewport.TopLeftY = 0; // 左上 Y
		viewport.MinDepth = 0.0f; // 縮小深度 (0 でいい)
		viewport.MaxDepth = 1.0f; // 拡大深度 (1 でいい)
		// ビューポート設定コマンドを、コマンドリストに積む
		dx->myCmdList.CommandList()->RSSetViewports(1, &viewport);

		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // 切り抜き座標 左
		scissorRect.right = scissorRect.left + WIN_SIZE.x; // 切り抜き座標 右
		scissorRect.top = 0; // 切り抜き座標 上
		scissorRect.bottom = scissorRect.top + WIN_SIZE.y; // 切り抜き座標 下
		// シザー矩形設定コマンドを、コマンドリストに積む
		dx->myCmdList.CommandList()->RSSetScissorRects(1, &scissorRect);

		// パイプラインステートとルートシグネチャの設定コマンド
		dx->myCmdList.CommandList()->SetPipelineState(pipelineState);
		dx->myCmdList.CommandList()->SetGraphicsRootSignature(rootSignature);

		// プリミティブ形状の設定コマンド
		dx->myCmdList.CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		dx->myCmdList.CommandList()->IASetVertexBuffers(0, 1, &vbView);

		// 頂点バッファビューの設定コマンド
		dx->myCmdList.CommandList()->IASetIndexBuffer(&ibView);

		// 定数バッファ
		dx->myCmdList.CommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		// SRVヒープの設定コマンド
		dx->myCmdList.CommandList()->SetDescriptorHeaps(1, &srvHeap);
		// SRVヒープの先頭ハンドルを取得 (SRVを指定しているはず)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		dx->myCmdList.CommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		// 定数バッファビュー(CBV)の設定コマンド (0)
		dx->myCmdList.CommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		// 描画コマンド
		//dx.myCmdList.CommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
		dx->myCmdList.CommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // 全ての頂点を使って描画

		// 定数バッファビュー(CBV)の設定コマンド (1)
		dx->myCmdList.CommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		// 描画コマンド
		dx->myCmdList.CommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // 全ての頂点を使って描画

		// -------------------------------------------------- //
		// 描画後処理
		dx->PostDraw();

		// ---------------------------------- //
	}

	// ウィンドウクラスを登録解除
	win->FinalProcess();

	return 0;
}