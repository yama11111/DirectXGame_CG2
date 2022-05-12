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

// エントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Windows 初期化
	MyWIndowsApplication win;
	win.CreateMyWindow(L"DirectXGame", { 1280,720 });

	// DirectX 初期化
	MyDirectX dx;
	dx.EnableDebugLayer();
	dx.Init(win.HandleWindow());

	// DirectInput 初期化
	MyDirectInput* input = MyDirectInput::GetInstance();
	input->Init(win.HandleWindowInstance(), win.HandleWindow());
	Keys* keys = Keys::GetInstance();

	// ------------------------------ //

	// ------- 描画初期化処理 ------- //
	// 頂点データ
	XMFLOAT3 vertices[] =
	{
		{ -0.5f, -0.5f, 0.0f }, // 左下
		{ -0.5f, +0.5f, 0.0f }, // 左上
		{ +0.5f, -0.5f, 0.0f }, // 右下
	};
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

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
	Result(dx.myDvc.Device()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	XMFLOAT3* vertMap = nullptr;
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
	vbView.StrideInBytes = sizeof(XMFLOAT3);

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
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
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

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	Result(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob));
	Result(dx.myDvc.Device()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	Result(dx.myDvc.Device()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	// ------------------------------ //

	// ゲームループ
	while (true)
	{
		// ×ボタンで終了メッセージが来たらゲームループを抜ける
		if (win.MessageLoop()) break;

		// ----- DirectX 毎フレーム処理 ----- //

		// -------------------- Update -------------------- //
		keys->Update();

		
		// ------------------------------------------------ //

		// 描画準備
		dx.PreDraw();
		// --------------------- Draw --------------------- //
		
		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		viewport.Width = win.SIZE->x; // 横幅
		viewport.Height = win.SIZE->y; // 縦幅
		viewport.TopLeftX = 0; // 左上 X
		viewport.TopLeftY = 0; // 左上 Y
		viewport.MinDepth = 0.0f; // 縮小深度 (0 でいい)
		viewport.MaxDepth = 1.0f; // 拡大深度 (1 でいい)
		// ビューポート設定コマンドを、コマンドリストに積む
		dx.myCmdList.CommandList()->RSSetViewports(1, &viewport);

		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // 切り抜き座標 左
		scissorRect.right = scissorRect.left + win.SIZE->x; // 切り抜き座標 右
		scissorRect.top = 0; // 切り抜き座標 上
		scissorRect.bottom = scissorRect.top + win.SIZE->y; // 切り抜き座標 下
		// シザー矩形設定コマンドを、コマンドリストに積む
		dx.myCmdList.CommandList()->RSSetScissorRects(1, &scissorRect);

		// パイプラインステートとルートシグネチャの設定コマンド
		dx.myCmdList.CommandList()->SetPipelineState(pipelineState);
		dx.myCmdList.CommandList()->SetGraphicsRootSignature(rootSignature);

		// プリミティブ形状の設定コマンド
		dx.myCmdList.CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		dx.myCmdList.CommandList()->IASetVertexBuffers(0, 1, &vbView);

		// 描画コマンド
		dx.myCmdList.CommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
		
		// -------------------------------------------------- //
		// 描画後処理
		dx.PostDraw();

		// ---------------------------------- //
	}

	// ウィンドウクラスを登録解除
	win.FinalProcess();

	return 0;
}