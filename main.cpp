#include <Windows.h>

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