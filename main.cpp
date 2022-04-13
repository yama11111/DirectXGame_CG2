#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージ毎にゲーム固有の処理
	switch (msg)
	{
		//ウインドウ破棄時
	case WM_DESTROY:
		//OSにアプリ終了伝達
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// エントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	OutputDebugStringA("Hello,DirectX!!\n");

	// ------- ウィンドウ設定 ------- //
	// ウィンドウサイズ 
	const int window_width = 1280; // 横
	const int window_height = 720; // 縦
	// ウィンドウクラス設定
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;	 // ウィンドウプロシージャ設定
	w.lpszClassName = L"DirectXGame";		 // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);  // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定
	// ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
	// ウィンドウサイズ {x, y, width，height}
	RECT wrc = { 0, 0, window_width, window_height };
	// 自動でサイスを補正
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	// ------------------------------ //
	// ------- ウィンドウ生成 ------- //
	// ウィンドウオブジェクトの生成
	HWND hwnd = CreateWindow(
		w.lpszClassName,	  // クラス名
		L"DirectXGame",		  // タイトルバー
		WS_OVERLAPPEDWINDOW,  // 標準的なウィンドウスタイル
		CW_USEDEFAULT,		  // 表示 X 座標 (OSに任せる)
		CW_USEDEFAULT,		  // 表示 Y 座標 (OSに任せる)
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr,			  // 親ウィンドウハンドル
		nullptr,			  // メニューハンドル
		w.hInstance,		  // 呼び出しアプリケーションハンドル
		nullptr);			  // オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);
	// ------------------------------ //

	MSG msg{}; // メッセージ

	// ----- DirectX 初期化処理 ----- //
	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	// DXGIファクトリー生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// アダプターの処理
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定の名前のアダプターが入る
	IDXGIAdapter4* tmpAdapter = nullptr;

	// パフォーマンスが高いものから順に、すべてのアダプターを列挙
	for (UINT i = 0; i < dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// 動的配列に追加
		adapters.push_back(tmpAdapter);
	}
	// 妥当なアダプタを選別
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイス生成
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	// コマンドアロケータ生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));

	// コマンドリスト生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	// コマンドキュー設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	// コマンドキュー生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

	// ------------------------------ //

	// ゲームループ
	while(true)
	{
		// メッセージがある？
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // キー入力メッセージの処理
			DispatchMessage(&msg); // プロシージにメッセージを送る
		}

		// ×ボタンで終了メッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT)
		{
			break;
		}

		// ----- DirectX 毎フレーム処理 ----- //

		// ---------------------------------- //
	}

	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}