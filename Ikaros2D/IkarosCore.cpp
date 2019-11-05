#include "IkarosCore.h"

//WindowProperties
#define CLASS_NAME     "MainWindow"
#define WINDOW_CAPTION "Ikaros2D"

static LPDIRECT3D9 g_pD3D = NULL;             //Direct3D interface
static LPDIRECT3DDEVICE9 pD3DDevice = NULL;  //Direct3D device

static HWND g_hWnd; //Window Handler

//Class static definitions
std::vector<Object*> Object::ObjectList{};
std::vector<TextureIndexData>Sprite::TextureList{};
std::vector<GameObject*> GameObject::GameObjectList{};
std::vector<Renderer*> Renderer::RendererList{};
std::vector<Camera*> Camera::CameraList{};

LPDIRECT3DDEVICE9 Camera::pD3DDevice;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool Initialize(HINSTANCE hInst);
static void Finalize(void);
static void Update(void);
static void Draw(void);
static bool D3D_Initialize(HWND hWnd);
static void D3D_Finalize();

int TickCount = 0;

#ifndef _BUILD
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//NEED UPDATE: need to redirect printf output to a file for release version
#ifdef _DEBUG
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif
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
		puts("ERROR::g_hWnd is NULL");
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
			//NEED UPDATE: Want an outer loop for scene management
			Update();
			Draw();
			TickCount++;
		}
	}
	Finalize();

	return (int)msg.wParam;
}
#elif _BUILD
int main() {
	//NEED UPDATE: BuilderCode
	return 0;
}
#endif

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

bool D3D_Initialize(HWND hWnd)
{
	// Direct3D Interface
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL) {
		puts("ERROR::Failed to create D3D Interface");
		return false;
	}

	// D3D parameters
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3D Device
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice))) {
		puts("ERROR::Failed to create D3D Device");
		return false;
	}

	Camera::pD3DDevice = pD3DDevice;

	return true;
}

void Transform::SetMatrix() {
	D3DXMATRIX MatTemp;

	D3DXMatrixScaling(&MatTemp, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&localToWorldMatrix, &localToWorldMatrix, &MatTemp);
	D3DXMatrixRotationYawPitchRoll(&MatTemp, rotation.y, rotation.x, rotation.z);
	D3DXMatrixMultiply(&localToWorldMatrix, &localToWorldMatrix, &MatTemp);
	D3DXMatrixTranslation(&MatTemp, position.x, position.y, position.z);
	D3DXMatrixMultiply(&localToWorldMatrix, &localToWorldMatrix, &MatTemp);
}

//File formats : .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga
bool Sprite::CreateTexture(LPCTSTR FilePath, LPDIRECT3DTEXTURE9* texturedata)
{
	HRESULT hr = D3DXCreateTextureFromFile(pD3DDevice, FilePath, texturedata);
	if (hr != D3D_OK){
		switch (hr) {
		case D3DERR_NOTAVAILABLE:
			std::cout << "ERROR::Failed to load Texture: " << FilePath << std::endl << "ERROR::NOTAVAILABLE" << std::endl;
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			std::cout << "ERROR::Failed to load Texture: " << FilePath << std::endl << "ERROR::OUTOFVIDEOMEMORY" << std::endl;
			break;
		case D3DERR_INVALIDCALL:
			std::cout << "ERROR::Failed to load Texture: " << FilePath << std::endl << "ERROR::INVALIDCALL" << std::endl;
			break;
		case D3DXERR_INVALIDDATA:
			std::cout << "ERROR::Failed to load Texture: " << FilePath << std::endl << "ERROR::INVALIDDATA" << std::endl;
			break;
		case E_OUTOFMEMORY:
			std::cout << "ERROR::Failed to load Texture: " << FilePath << std::endl << "ERROR::E_OUTOFMEMORY" << std::endl;
			break;
		default:
			std::cout << "ERROR::Failed to load Texture: " << FilePath << std::endl << "ERROR::Unknown Error" << std::endl;
		}
		return false;

	}else {
		return true;

	}
}

void Sprite::SetTexture(std::string Name) {
	texture = FindTexturebyName(Name);

	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VertexBufferData),
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&VertexBuffer,
		NULL)))
	{
		std::cout << "ERROR::Failed to create vertex buffer for object: " << parent->name << std::endl;
	}
	else {
		vertices = nullptr;
		VertexBuffer->Lock(0,0, (void**)&vertices, 0);

		vertices[0].position = Vector3(-0.5f, 0.5f, 0);
		vertices[1].position = Vector3(0.5f, 0.5f, 0);
		vertices[2].position = Vector3(-0.5f, -0.5f, 0);
		vertices[3].position = Vector3(0.5f, -0.5f, 0);

		vertices[0].uv = Vector2(0, 0);
		vertices[1].uv = Vector2(1, 0);
		vertices[2].uv = Vector2(0, 1);
		vertices[3].uv = Vector2(1, 1);

		for (int i = 0; i < 4; i++) {
			vertices[i].position.x *= texture.Width;
			vertices[i].position.y *= texture.Height;

			vertices[i].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			vertices[i].normal = Vector3(0, 0, 0);
		}

		VertexBuffer->Unlock();
	}
}

//Init renderer
Renderer::Renderer() : Component("Renderer") {
	sprite = nullptr;
	sortingOrder = 0;

	RendererList.push_back(this);
}

//Main draw function for each camera
void Camera::draw() {
	pD3DDevice->SetTransform(D3DTS_VIEW, &View);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &Projection);
	for (auto p : Renderer::RendererList) {
		pD3DDevice->SetTransform(D3DTS_WORLD, &static_cast<GameObject*>(p->parent)->transform->localToWorldMatrix);

		pD3DDevice->SetStreamSource(0, p->sprite->VertexBuffer, 0, sizeof(VertexBufferData));
		pD3DDevice->SetTexture(0, *(p->sprite->texture.texturedata));
		pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
	}
}

void Camera::SetD3DDevice() {
	//Turn off lighting
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);

	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

	/*
	Tile the texture at every integer junction. For example, for u values between 0 and 3, the texture is repeated three times; no mirroring is performed.
	*/
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	/*
	Similar to D3DTADDRESS_WRAP, except that the texture is flipped at every integer junction.
	For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored);
	between 2 and 3, the texture is normal again; and so on.
	*/
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	/*
	Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
	*/
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	/*
	Texture coordinates outside the range [0.0, 1.0] are set to the border color.
	*/
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	pD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 0, 0, 255));

	/*
	When used with D3DSAMP_ MAGFILTER or D3DSAMP_MINFILTER,
	specifies that point filtering is to be used as the texture magnification or
	minification filter respectively. When used with D3DSAMP_MIPFILTER,
	enables mipmapping and specifies that the rasterizer chooses the color from the texel of the nearest mip level.
	*/
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	/*
	When used with D3DSAMP_ MAGFILTER or D3DSAMP_MINFILTER,
	specifies that linear filtering is to be used as the texture magnification or
	minification filter respectively. When used with D3DSAMP_MIPFILTER, enables mipmapping and trilinear filtering;
	it specifies that the rasterizer interpolates between the two nearest mip levels.
	*/
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	/*
	When used with D3DSAMP_ MAGFILTER or D3DSAMP_MINFILTER,
	specifies that anisotropic texture filtering used as a texture magnification or
	minification filter respectively. Compensates for distortion caused by the difference in angle between the texture polygon
	and the plane of the screen. Use with D3DSAMP_MIPFILTER is undefined.
	*/
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	//	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);

	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x80);
	//	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// Alpha blend settings
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

	// D3DTEXTUREOP Texture blending settings
	//	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

// Uninitialization
void D3D_Finalize(void)
{
	//Device
	if (pD3DDevice) {
		pD3DDevice->Release();
		pD3DDevice = NULL;
	}
	// Interface
	if (g_pD3D != NULL) {
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

GameObject* test;
bool Initialize(HINSTANCE hInst)
{
	//Initilize RNG Seed
	srand((unsigned int)time(NULL));

	if (!D3D_Initialize(g_hWnd)) {
		puts("Failed to initilize D3D");
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

	//Fade_Initialize();
	//Scene_Initialize(SCENE_INDEX_TITLE);
	//Collision_Initialize();

	test = new GameObject("test");
	test->AddComponent("RigidBody");
	test->AddComponent("Renderer");
	
	Renderer* Rtest = static_cast<Renderer*>(test->GetComponent("RigidBody"));
	return true;
}

void Update(void)
{
	printf("Object number %d \n GameObject number %d\n", Object::GetSize(), GameObject::GetSize());
	//キーボードの状態を更新する
	//Keyboard_Update();

	//キーボードの状態を更新する
	//GamePad_Update();

	//シーンの更新
	//Scene_Update();

	//フェードの更新
	//Fade_Update();
}

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

void Finalize(void)
{
	Sprite::ReleaseTextures();

	// DirectInputの終了処理
	//GamePad_Finalize();

	// DirectInputの終了処理
	//Keyboard_Finalize();

	// Kill D3D
	D3D_Finalize();
}