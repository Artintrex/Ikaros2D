#include "IkarosCore.h"
#include "SceneManager.h"
#include "GameHeader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//WindowProperties
#define CLASS_NAME     "MainWindow"
#define WINDOW_CAPTION "Ikaros2D"

static LPDIRECT3D9 g_pD3D = NULL;             //Direct3D interface
static LPDIRECT3DDEVICE9 pD3DDevice = NULL;  //Direct3D device
static D3DVIEWPORT9 MainViewport;

static HWND g_hWnd; //Window Handler

//Class static definitions
std::vector<Object*> Object::ObjectList{};
std::vector<Texture*> Texture::TextureList{};
std::vector<Transform*> Transform::TransformList{};
std::vector<GameObject*> GameObject::GameObjectList{};
std::vector<Renderer*> Renderer::RendererList{};
std::vector<Camera*> Camera::CameraList{};
std::vector<MonoBehavior*> MonoBehavior::MonoBehaviorList{};
std::vector<RigidBody*> RigidBody::RigidBodyList{};
std::map<std::string, ComponentFactory*> Component::factories{};
std::map<std::string, bool> MonoBehavior::isAwake{};

iTime Time;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool Initialize(HINSTANCE hInstance);
static void Finalize();
static bool D3D_Initialize();
static void D3D_Finalize();

static void GameLoop();

b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);

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
	MainViewport = {0, 0, SCREEN_WIDTH , SCREEN_HEIGHT , 0, 1};

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

	//NEED UPDATE: DEBUG CODE
	//GameObject* test = new GameObject("GameManager");
	//test->AddComponent<GameManager>();

	SceneManager::LoadScene(0);

	//NEED UPDATE: Move this after adding scene loader and add Awake as well
	//MonoBehavior::StartMonoBehaviorArray(); //Start MonoBehavior

	//Main Loop
	MSG msg = {};
	while (WM_QUIT != msg.message) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			GameLoop();
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
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

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

		//catch mousewheel event
	case WM_MOUSEWHEEL:
		mouseScrollDelta += (short)HIWORD(wParam) / 120;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool D3D_Initialize()
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

	return true;
}

//Class Method Definitions
Object::Object(std::string Name) {
	name = Name;
	ObjectList.push_back(this);
}

Object::~Object() {
	ObjectList.erase(std::remove(ObjectList.begin(), ObjectList.end(), this), ObjectList.end());
}

void Object::ReleaseObjects() {

	while (ObjectList.size() > 0) {
		std::cout << ObjectList[0]->name << std::endl;
		std::cout << ObjectList.size() << std::endl;
		delete ObjectList.at(0);
	}

	ObjectList.clear();
}

Component::Component(std::string Name) : Object(Name) {
	parent = nullptr;
	transform = nullptr;
}
Component::~Component() {
	if (parent != nullptr) {
		for (std::vector<Component*>::iterator it = parent->ComponentList.begin(); it != parent->ComponentList.end(); ++it)
		{
			if (this == (*it)) {
				parent->ComponentList.erase(it);
				break;
			}
		}
	}
}

MonoBehavior::MonoBehavior(std::string Name) : Behavior(Name) {
	MonoBehaviorList.push_back(this);
}

MonoBehavior::~MonoBehavior() {
	for (std::vector<MonoBehavior*>::iterator it = MonoBehaviorList.begin(); it != MonoBehaviorList.end(); ++it)
	{
		if (this == (*it)) {
			MonoBehaviorList.erase(it);
			break;
		}
	}
}

void MonoBehavior::StartMonoBehaviorArray() {
	std::vector<MonoBehavior*>::size_type size = MonoBehaviorList.size();
	for (int i = 0; i < size; ++i) {
		MonoBehaviorList[i]->Start();
		size = MonoBehaviorList.size();
	}
}

void MonoBehavior::UpdateMonoBehaviorArray() {
	std::vector<MonoBehavior*>::size_type size = MonoBehaviorList.size();
	for (int i = 0; i < size; ++i) {
		MonoBehaviorList[i]->Update();
		size = MonoBehaviorList.size();
	}
}

Texture::Texture(std::string Name) : Object(Name) {
	texturedata = nullptr;
	Width = 0;
	Height = 0;
}

Texture::~Texture() {
	for (std::vector<Texture*>::iterator it = TextureList.begin(); it != TextureList.end(); ++it)
	{
		if (this == (*it)) {
			(*texturedata)->Release();
			TextureList.erase(it);
			break;
		}
	}
}

//File formats : .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga
Texture* Texture::LoadTexture(std::string TextureName, LPCTSTR FilePath) {
		LPDIRECT3DTEXTURE9* tex = new LPDIRECT3DTEXTURE9;
		if (CreateTexture(FilePath, tex)) {
			Texture* texture = new Texture(TextureName);
			texture->texturedata = tex;

			stbi_info(FilePath, &(texture->Width), &(texture->Height), 0);

			TextureList.push_back(texture);

			return texture;
		}
		else delete tex; return nullptr;
}

Texture* Texture::FindTexturebyName(std::string Name) {
	for (std::vector<Texture*>::iterator it = TextureList.begin(); it != TextureList.end(); ++it)
	{
		if (Name == (*it)->name)return *it;
	}
	return nullptr;
}

bool Texture::CreateTexture(LPCTSTR FilePath, LPDIRECT3DTEXTURE9* texturedata)
{
	HRESULT hr = D3DXCreateTextureFromFile(pD3DDevice, FilePath, texturedata);
	if (hr != D3D_OK) {
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

	}
	else {
		return true;

	}
}

Sprite::Sprite(Texture* pTexture, std::string Name) : Object(Name) {
	renderer = nullptr;
	texture = pTexture;
	vertices = nullptr;
	indices = nullptr;
	VertexBuffer = NULL;
	IndexBuffer = NULL;

	color = D3DCOLOR_RGBA(255, 255, 255, 255);

	if (texture != nullptr) {
		GenereteSprite();
	}
}

Sprite::~Sprite() {
	VertexBuffer->Release();
	IndexBuffer->Release();
}

void Sprite::GenereteSprite(std::string Name) {
	if (texture == nullptr) {
		texture = texture->FindTexturebyName(Name);
	}
	
	if (texture != nullptr) {

		if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VertexBufferData) * 4,
			D3DUSAGE_WRITEONLY,
			(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1),
			D3DPOOL_MANAGED,
			&VertexBuffer,
			NULL)))
		{
			std::cout << "ERROR::Failed to create vertex buffer for Texture: " << Name << std::endl;
		}
		else {
			VertexBuffer->Lock(0, 0, (void**)&vertices, 0);

			vertices[0].position = Vector3(-0.5f, 0.5f, 0);
			vertices[1].position = Vector3(0.5f, 0.5f, 0);
			vertices[2].position = Vector3(-0.5f, -0.5f, 0);
			vertices[3].position = Vector3(0.5f, -0.5f, 0);

			vertices[0].uv = Vector2(0, 0);
			vertices[1].uv = Vector2(1, 0);
			vertices[2].uv = Vector2(0, 1);
			vertices[3].uv = Vector2(1, 1);

			size = Vector2((float)texture->Width / 100, (float)texture->Height / 100);

			for (int i = 0; i < 4; i++) {
				vertices[i].position.x *= size.x;
				vertices[i].position.y *= size.y;

				vertices[i].color = color;
			}

			VertexBuffer->Unlock();
		}

		if (FAILED(pD3DDevice->CreateIndexBuffer(sizeof(WORD) * 12,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&IndexBuffer,
			NULL)))
		{
			std::cout << "ERROR::Failed to create index buffer for Texture: " << Name << std::endl;
		}
		else {
			IndexBuffer->Lock(0, 0, (void**)&indices, 0);
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;

			indices[3] = 2;
			indices[4] = 3;
			indices[5] = 1;

			indices[6] = 3;
			indices[7] = 2;
			indices[8] = 1;

			indices[9] = 1;
			indices[10] = 0;
			indices[11] = 2;

			IndexBuffer->Unlock();
		}
	}
	else std::cout << this->name << "can`t find texture" << std::endl;
}

void Sprite::SetColor(D3DCOLOR rbga) {
	color = rbga;
	GenereteSprite();
}

Renderer::Renderer(GameObject* Parent, std::string Name) : Component(Name) {
	parent = Parent;
	transform = Parent->transform;
	sprite = nullptr;
	sortingOrder = 0;

	RendererList.push_back(this);
}

Renderer::~Renderer() {
	for (std::vector<Renderer*>::iterator it = RendererList.begin(); it != RendererList.end(); ++it)
	{
		if (this == (*it)) {
			RendererList.erase(it);
			break;
		}
	}
}

void Transform::SetMatrix() {
	D3DXMATRIX MatTemp;

	D3DXMatrixIdentity(&localToWorldMatrix);
	D3DXMatrixScaling(&MatTemp, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&localToWorldMatrix, &localToWorldMatrix, &MatTemp);
	D3DXMatrixRotationYawPitchRoll(&MatTemp, rotation.y, rotation.x, rotation.z);
	D3DXMatrixMultiply(&localToWorldMatrix, &localToWorldMatrix, &MatTemp);
	D3DXMatrixTranslation(&MatTemp, position.x, position.y, position.z);
	D3DXMatrixMultiply(&localToWorldMatrix, &localToWorldMatrix, &MatTemp);

	oldposition = position;
	oldrotation = rotation;
	oldscale = scale;
}

Transform::Transform(Vector3 Position, Vector3 Rotation, Vector3 Scale) : Component("Transform") {
	position = Position;
	rotation = Rotation;
	scale = Scale;

	oldposition = position;
	oldrotation = rotation;
	oldscale = scale;

	D3DXMatrixIdentity(&localToWorldMatrix);

	SetMatrix();

	TransformList.push_back(this);
}

Transform::~Transform() {
	for (std::vector<Transform*>::iterator it = TransformList.begin(); it != TransformList.end(); ++it)
	{
		if (this == (*it)) {
			TransformList.erase(it);
			break;
		}
	}
}

void Transform::Translate(float x, float y, float z) {
	position += Vector3(x, y, z);
}

void Transform::Rotate(float x, float y, float z) {
	rotation += Vector3(D3DXToRadian(x), D3DXToRadian(y), D3DXToRadian(z));
}

void Transform::Scale(float x, float y, float z) {
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
}

void Transform::UpdateTransform() {
	for (auto p : TransformList) {
		if (p->position != p->oldposition ||
			p->rotation != p->oldrotation ||
			p->scale != p->oldscale) {
			p->SetMatrix();
		}
	}
}

Camera::Camera(GameObject* Parent, std::string Name) : Behavior(Name) {
	parent = Parent;
	transform = Parent->transform;

	fieldOfView = (D3DXToRadian(60.0f));
	nearClipPlane = 0.3f;
	farClipPlane = 10000.0f;
	rect.X = 0;
	rect.Y = 0;
	rect.W = 1;
	rect.H = 1;
	aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	orthographic = false;
	orthographicSize = 5.0f;

	D3DXMatrixIdentity(&mCameraWorld);
	D3DXMatrixIdentity(&Projection);
	D3DXMatrixIdentity(&View);

	SetProjection();

	CameraList.push_back(this);
}

Camera::~Camera() {
	for (std::vector<Camera*>::iterator it = CameraList.begin(); it != CameraList.end(); ++it)
	{
		if (this == (*it)) {
			CameraList.erase(it);
			break;
		}
	}
}

//Draw all cameras
void Camera::Draw() {
	pD3DDevice->SetViewport(&MainViewport);
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(50, 50, 50, 255), 1.0f, 0);
	for (auto p : CameraList) {
		p->draw();
	}
}

//Main draw function for a camera
void Camera::draw() {
	SetD3DDevice();
	SetCamera();

	pD3DDevice->SetTransform(D3DTS_VIEW, &View);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &Projection);

	
	for (auto p : Renderer::RendererList) {
		if (p->sprite == nullptr) {
			std::cout << p->parent->name << "has renderer with no sprite set" << std::endl;
			continue;
		}
		pD3DDevice->SetTransform(D3DTS_WORLD, &static_cast<GameObject*>(p->parent)->transform->localToWorldMatrix);

		pD3DDevice->SetStreamSource(0, p->sprite->VertexBuffer, 0, sizeof(VertexBufferData));
		pD3DDevice->SetIndices(p->sprite->IndexBuffer);

		pD3DDevice->SetTexture(0, *(p->sprite->texture->texturedata));
	
		if(pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 4) != D3D_OK){
			puts("Failed to Render");
		}
	}
}

void Camera::SetViewport() {
	viewport.X = rect.X * SCREEN_WIDTH;
	viewport.Width = rect.W * SCREEN_WIDTH;
	viewport.Y = rect.Y * SCREEN_HEIGHT;
	viewport.Height = rect.H * SCREEN_HEIGHT;
	viewport.MinZ = nearClipPlane;
	viewport.MaxZ = farClipPlane;
}

void Camera::SetD3DDevice() {
	pD3DDevice->SetViewport(&viewport);

	pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

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
	//pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	/*
	Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
	*/
	//pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	/*
	Texture coordinates outside the range [0.0, 1.0] are set to the border color.
	*/
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(0, 0, 0, 255));

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

void Camera::SetProjection() {
	SetViewport();

	D3DXMatrixPerspectiveFovLH(&Projection,
		fieldOfView,
		aspect,
		nearClipPlane,
		farClipPlane);
	if (orthographic) {
		D3DXMatrixIdentity(&Projection);

		D3DXMatrixOrthoLH(&Projection,
			SCREEN_WIDTH * rect.W * orthographicSize,
			SCREEN_HEIGHT * rect.H * orthographicSize,
			nearClipPlane,
			farClipPlane);
	}
}

void Camera::SetCamera() {

	D3DXMatrixRotationYawPitchRoll(&mCameraRot, transform->rotation.y, transform->rotation.x, transform->rotation.z);

	Vector3 vWorldUp, vWorldAhead;
	Vector3 vLocalUp(0, 1, 0);
	Vector3 vLocalAhead(0, 0, 1);

	D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
	D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);

	//Local to World Coord for relative camera translation
	//D3DXVec3TransformCoord(&vPosWorld, &vDelta, &mCameraRot);
	//add postworld to transform for movement

	Vector3 LookAt = transform->position + vWorldAhead;

	D3DXMatrixLookAtLH(&View, &(transform->position), &LookAt, &vWorldUp);
	D3DXMatrixInverse(&mCameraWorld, NULL, &View);
}

RigidBody::RigidBody(GameObject* Parent, std::string Name) : Component(Name) {
	parent = Parent;
	transform = Parent->transform;

	b2BodyDef bodydefinition;
	bodydefinition.position.Set(transform->position.x, transform->position.y);
	rigidbody = world.CreateBody(&bodydefinition);

	RigidBodyList.push_back(this);
}

RigidBody::~RigidBody() {
	world.DestroyBody(rigidbody);

	for (std::vector<RigidBody*>::iterator it = RigidBodyList.begin(); it != RigidBodyList.end(); ++it)
	{
		if (this == (*it)) {
			RigidBodyList.erase(it);
			break;
		}
	}
}

void RigidBody::UpdateRigidBody() {
	for (auto p : RigidBodyList) {
		p->transform->position.x = p->rigidbody->GetPosition().x;
		p->transform->position.y = p->rigidbody->GetPosition().y;
		p->transform->rotation.z = p->rigidbody->GetAngle();

		p->velocity.x = p->rigidbody->GetLinearVelocity().x;
		p->velocity.y = p->rigidbody->GetLinearVelocity().y;
	}
}

void RigidBody::AddForce(Vector2 force, ForceMode mode) {
	switch (mode) {
	case Force:
		rigidbody->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
		break;
	case Impulse:
		rigidbody->ApplyLinearImpulseToCenter(b2Vec2(force.x, force.y), true);
		break;
	case VelocityChange:
		rigidbody->SetLinearVelocity(b2Vec2(force.x, force.y));
		break;
	}
}

void RigidBody::AddTorque(float torque, ForceMode mode) {
	switch (mode) {
	case Force:
		rigidbody->ApplyTorque(torque, true);
		break;
	case Impulse:
		rigidbody->ApplyAngularImpulse(torque, true);
		break;
	case VelocityChange:
		rigidbody->SetAngularVelocity(torque);
		break;
	}
}

void RigidBody::AddCircleCollider(float radius, Vector2 center, float density, float friction, float bounciness, uint16 maskBits, bool isTrigger) {
	b2CircleShape circle;
	circle.m_p.Set(center.x, center.y);
	circle.m_radius = radius;

	b2FixtureDef fixtureDef;

	fixtureDef.shape = &circle;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = bounciness;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.isSensor = isTrigger;

	b2Fixture* collider = rigidbody->CreateFixture(&fixtureDef);

	ColliderList.push_back(collider);
}

void RigidBody::AddBoxCollider(Vector2 size, Vector2 center, float angle, float density, float friction, float bounciness, uint16 maskBits, bool isTrigger) {
	b2PolygonShape box;
	box.SetAsBox(size.x / 2, size.y / 2, b2Vec2(center.x, center.y), angle);
	b2FixtureDef fixtureDef;

	fixtureDef.shape = &box;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = bounciness;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.isSensor = isTrigger;

	b2Fixture* collider = rigidbody->CreateFixture(&fixtureDef);

	ColliderList.push_back(collider);
}

GameObject::GameObject(std::string Name) : Object(Name) {
	transform = new Transform();
	transform->parent = this;
	GameObjectList.push_back(this);
}

GameObject::~GameObject() {
	delete transform;

	for (int j = 0, i = ComponentList.size(); j < i; j++) {
		delete ComponentList.at(0);
	}

	ComponentList.clear();

	for (std::vector<GameObject*>::iterator it = GameObjectList.begin(); it != GameObjectList.end(); ++it)
	{
		if (this == (*it)) {
			GameObjectList.erase(it);
			break;
		}
	}
}

GameObject* GameObject::Find(std::string Name) {
	for (std::vector<GameObject*>::iterator it = GameObjectList.begin(); it != GameObjectList.end(); ++it)
	{
		if (Name == (*it)->name)return *it;
	}
	return nullptr;
}


// Uninitialization
void D3D_Finalize()
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

bool Initialize(HINSTANCE hInstance)
{
	//Initilize RNG Seed
	srand((unsigned int)time(NULL));

	if (!D3D_Initialize()) {
		puts("Failed to initilize D3D");
		return false;
	}

	InputInitialize(hInstance, g_hWnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.Fonts->AddFontDefault();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(pD3DDevice);
	return true;
}

void Finalize(void)
{
	SceneManager::UnloadScene();

	InputRelease();

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Kill D3D
	D3D_Finalize();
}

void GameLoop() {
	Time.Start();
	//NEED UPDATE: Want an outer loop for scene management

	world.Step(Time.DeltaTime, 6, 2); //Physics engine iteration
	RigidBody::UpdateRigidBody(); //Update transforms to physics engine output


	InputUpdate(); //Update keyboard, mouse and gamepad inputs

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	MonoBehavior::UpdateMonoBehaviorArray(); //Update MonoBehavior
	ImGui::EndFrame();
	Transform::UpdateTransform(); //Update matrices if transforms are changed


	//
	//Begin drawing
	//

	pD3DDevice->BeginScene();

	Camera::Draw();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	//End drawing
	pD3DDevice->EndScene();

	// Back buffer flipiTiming depends on D3DPRESENT_PARAMETERSj
	pD3DDevice->Present(NULL, NULL, NULL, NULL);

	Time.End();
}