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

	// DirectInput�̏������i�L�[�{�[�h�j
	//if (!Keyboard_Initialize(hInst, g_hWnd)) {
	//	return false;
	//}
	// DirectInput�̏������i�Q�[���p�b�h�j
	//if (!GamePad_Initialize(hInst, g_hWnd)) {
	//	return false;
	//}

	//�S�e�N�X�`���t�@�C���̃��[�h
	//Texture_Load();

	//Fade_Initialize();
	//Scene_Initialize(SCENE_INDEX_TITLE);
	//Collision_Initialize();

	return true;
}

void Finalize(void)
{
	// �e�N�X�`���̉��
	//Texture_Release();

	// DirectInput�̏I������
	//GamePad_Finalize();

	// DirectInput�̏I������
	//Keyboard_Finalize();

	// �Q�[���̏I������(Direct3D�̏I������)
	D3D_Finalize();
}

// �Q�[���̍X�V�֐�
void Update(void)
{
	//�L�[�{�[�h�̏�Ԃ��X�V����
	//Keyboard_Update();

	//�L�[�{�[�h�̏�Ԃ��X�V����
	//GamePad_Update();

	//�V�[���̍X�V
	//Scene_Update();

	//�t�F�[�h�̍X�V
	//Fade_Update();
}

// �Q�[���̕`��֐�
void Draw(void)
{

	// ��ʂ̃N���A
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(50, 50, 50, 255), 1.0f, 0);

	// �`��o�b�`���߂̊J�n
	pD3DDevice->BeginScene();


	// �V�[���̕`��
	//Scene_Draw();

	// �t�F�[�h�̕`��
	//Fade_Draw();


	// �`��o�b�`���߂̏I��
	pD3DDevice->EndScene();

	// �o�b�N�o�b�t�@���t���b�v�i�^�C�~���O��D3DPRESENT_PARAMETERS�̐ݒ�ɂ��j
	pD3DDevice->Present(NULL, NULL, NULL, NULL);


	// �V�[���̃`�F�b�N
	//Scene_Check();
}

//�E�B���h�E�n���h���̃Q�b�^�[
HWND GetHWND() {
	return g_hWnd;
}


double frand() {
	return (double)rand() / RAND_MAX;
}


//DirectX9



// �Q�[���̏������֐�
bool D3D_Initialize(HWND hWnd)
{
	// Direct3D�C���^�[�t�F�[�X�̎擾
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL) {
		// Direct3D�C���^�[�t�F�[�X�̎擾�Ɏ��s
		MessageBox(hWnd, "Direct3D�C���^�[�t�F�[�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	// �f�o�C�X�̐����ɗ��p������\���̂̍쐬
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = SCREEN_WIDTH;                // �o�b�N�o�b�t�@�̉���				
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;               // �o�b�N�o�b�t�@�̏c��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;              // �o�b�N�o�b�t�@�̃t�H�[�}�b�g�w��
	d3dpp.BackBufferCount = 1;                           // �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;       // �X���b�v�G�t�F�N�g�i�X���b�v���@�j�̐ݒ�		
	d3dpp.Windowed = TRUE;                        // �E�B���h�E���[�h or �t���X�N���[��
	d3dpp.EnableAutoDepthStencil = TRUE;                        // �[�x�o�b�t�@�E�X�e���V���o�b�t�@�̎g�p				
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // �[�x�o�b�t�@�E�X�e���V���o�b�t�@�̃t�H�[�}�b�g�w��
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;     // �t���X�N���[�����̃��t���b�V�����[�g�̎w��
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; // ���t���b�V�����[�g��Present�����̊֌W

	// Direct3D�f�o�C�X�̎擾
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetHWND(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice))) {
		// �f�o�C�X�̍쐬�Ɏ��s
		MessageBox(hWnd, "Direct3D�f�o�C�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	//���C�g���I�t�ɂ���
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);

	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

	//�A�h���b�V���O���[�h�F���b�v�i�摜���J��Ԃ��j
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�A�h���b�V���O���[�h�F�~���[�i�摜�𔽓]�����ČJ��Ԃ��j
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	//�A�h���b�V���O���[�h�F�N�����v�i�Ō�̃J���[�ňȍ~��h��Ԃ��j
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	//�A�h���b�V���O���[�h�F�{�[�_�[�i����̐F�œh��Ԃ��j
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	pD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 0, 0, 255));

	//�e�N�X�`���t�B���^�[�F�|�C���g
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	//�e�N�X�`���t�B���^�[�F�o�C���j�A�t�B���^
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//�e�N�X�`���t�B���^�[�F�ٕ����t�B���^
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);

	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x80);
	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);


	// �A���t�@�u�����h�̐ݒ�
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

	// ���_�J���[�ƃe�N�X�`���̃u�����h�ݒ�
	//	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return true;
}


// �Q�[���̏I������
void D3D_Finalize(void)
{
	// Direct3D�f�o�C�X�̉��
	if (pD3DDevice) {
		pD3DDevice->Release();
		pD3DDevice = NULL;
	}

	// Direct3D�C���^�[�t�F�[�X�̉��
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