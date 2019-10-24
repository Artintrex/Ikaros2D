#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Box2D\Box2D.h"

//WindowProperties
#define CLASS_NAME     "MainWindow"
#define WINDOW_CAPTION "Ikaros2D"
//Change Resolution Settings needs to read from file!!
#define SCREEN_WIDTH  (1024)
#define SCREEN_HEIGHT  (576)


static LPDIRECT3D9 g_pD3D = NULL;             //Direct3D interface
static LPDIRECT3DDEVICE9 pD3DDevice = NULL;  //Direct3D device

static HWND g_hWnd; //Window Handler

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool Initialize(HINSTANCE hInst);
static void Finalize(void);
static void Update(void);
static void Draw(void);
static bool D3D_Initialize(HWND hWnd);
static void D3D_Finalize();

int TickCount = 0;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Unused parameters
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Window Structure
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);

	// Window stle
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	AdjustWindowRect(&window_rect, window_style, FALSE);

	// Calculate window rect dimensions
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// Get desktop resolution
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//Center window
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	//Create window
	g_hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		window_style,
		window_x,
		window_y,
		window_width,
		window_height,
		NULL,           // Parent window handle
		NULL,           // Menu handle
		hInstance,      // Instance handle
		NULL            // Additional data
	);

	if (g_hWnd == NULL) {
		//Abort if handle NULL
		return -1;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	Initialize(hInstance);

	//Main Loop
	MSG msg = {};
	while (WM_QUIT != msg.message) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Update();
			Draw();
			TickCount++;
		}
	}
	Finalize();

	return (int)msg.wParam;
}

//Callback function
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
				if (MessageBox(hWnd, "Are you sure you want to quit?", WINDOW_CAPTION, MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
		DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Initialize(HINSTANCE hInst)
{
	//RNG Seed
	srand((unsigned int)time(NULL));

	if (!D3D_Initialize(g_hWnd)) {
		return false;
	}

	// DirectInputの初期化（キーボード）
	//if (!Keyboard_Initialize(hInst, g_hWnd)) {
	//	return false;
	//}
	// DirectInputの初期化（ゲームパッド）
	//if (!GamePad_Initialize(hInst, g_hWnd)) {
	//	return false;
	//}

	//全テクスチャファイルのロード
	//Texture_Load();

	//Fade_Initialize();
	//Scene_Initialize(SCENE_INDEX_TITLE);
	//Collision_Initialize();

	return true;
}

void Finalize(void)
{
	// テクスチャの解放
	//Texture_Release();

	// DirectInputの終了処理
	//GamePad_Finalize();

	// DirectInputの終了処理
	//Keyboard_Finalize();

	// ゲームの終了処理(Direct3Dの終了処理)
	D3D_Finalize();
}

// ゲームの更新関数
void Update(void)
{
	//キーボードの状態を更新する
	//Keyboard_Update();

	//キーボードの状態を更新する
	//GamePad_Update();

	//シーンの更新
	//Scene_Update();

	//フェードの更新
	//Fade_Update();
}

// ゲームの描画関数
void Draw(void)
{

	// 画面のクリア
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(50, 50, 50, 255), 1.0f, 0);

	// 描画バッチ命令の開始
	pD3DDevice->BeginScene();


	// シーンの描画
	//Scene_Draw();

	// フェードの描画
	//Fade_Draw();


	// 描画バッチ命令の終了
	pD3DDevice->EndScene();

	// バックバッファをフリップ（タイミングはD3DPRESENT_PARAMETERSの設定による）
	pD3DDevice->Present(NULL, NULL, NULL, NULL);


	// シーンのチェック
	//Scene_Check();
}

//ウィンドウハンドルのゲッター
HWND GetHWND() {
	return g_hWnd;
}


double frand() {
	return (double)rand() / RAND_MAX;
}


//DirectX9



// ゲームの初期化関数
bool D3D_Initialize(HWND hWnd)
{
	// Direct3Dインターフェースの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL) {
		// Direct3Dインターフェースの取得に失敗
		MessageBox(hWnd, "Direct3Dインターフェースの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	// デバイスの生成に利用する情報構造体の作成
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = SCREEN_WIDTH;                // バックバッファの横幅				
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;               // バックバッファの縦幅
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;              // バックバッファのフォーマット指定
	d3dpp.BackBufferCount = 1;                           // バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;       // スワップエフェクト（スワップ方法）の設定		
	d3dpp.Windowed = TRUE;                        // ウィンドウモード or フルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE;                        // 深度バッファ・ステンシルバッファの使用				
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // 深度バッファ・ステンシルバッファのフォーマット指定
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;     // フルスクリーン時のリフレッシュレートの指定
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; // リフレッシュレートとPresent処理の関係

	// Direct3Dデバイスの取得
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetHWND(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice))) {
		// デバイスの作成に失敗
		MessageBox(hWnd, "Direct3Dデバイスの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	//ライトをオフにする
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);

	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

	//アドレッシングモード：ラップ（画像を繰り返す）
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//アドレッシングモード：ミラー（画像を反転させて繰り返す）
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	//アドレッシングモード：クランプ（最後のカラーで以降を塗りつぶし）
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	//アドレッシングモード：ボーダー（特定の色で塗りつぶし）
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	pD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 0, 0, 255));

	//テクスチャフィルター：ポイント
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	//テクスチャフィルター：バイリニアフィルタ
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//テクスチャフィルター：異方性フィルタ
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);

	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x80);
	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);


	// アルファブレンドの設定
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_DIFFUSE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_DIFFUSE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// 頂点カラーとテクスチャのブレンド設定
	//	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return true;
}


// ゲームの終了処理
void D3D_Finalize(void)
{
	// Direct3Dデバイスの解放
	if (pD3DDevice) {
		pD3DDevice->Release();
		pD3DDevice = NULL;
	}

	// Direct3Dインターフェースの解放
	if (g_pD3D != NULL) {
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}



//load objects into a list

//update list

//draw list

//controller

//sound

//texture loader