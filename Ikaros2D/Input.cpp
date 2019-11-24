#include "Input.h"
#include "GameHeader.h"


#define DIRECTINPUT_VERSION (0x0800)

//Gamepad parameters
#define DEADZONE		2500
#define RANGE_MAX		10000
#define RANGE_MIN		-10000

int mouseScrollDelta;
POINT mousePosition;

LPDIRECTINPUT8			g_pInput = NULL;

//Keyboard globals
static LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;
static BYTE					g_aKeyState[NUM_KEY_MAX];
static BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];
static BYTE					g_aKeyStateRelease[NUM_KEY_MAX];

//Gamepad globals
static LPDIRECTINPUTDEVICE8	g_pGamePad[GAMEPADMAX] = { NULL, NULL, NULL, NULL };// Gamepad device
static DWORD				g_padState[GAMEPADMAX];	// State information for multiple gamepad
static DWORD				g_padTrigger[GAMEPADMAX];
static DWORD				g_padRelease[GAMEPADMAX];
static DWORD				g_padAxis[GAMEPADMAX];
static int					g_padCount = 0;


HWND MainWindowHandle;
HINSTANCE InstanceHandle;

bool InitDirectInput()
{
	if (g_pInput == NULL) {

		if (FAILED(DirectInput8Create(InstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) {

			return false;
		}
	}

	return true;
}

void ReleaseDirectInput(void)
{
	if (g_pInput != NULL) {
		g_pInput->Release();
		g_pInput = NULL;
	}
}

bool Keyboard_Initialize()
{
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		puts("Failed to create keyboard device");
		return false;
	}

	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		puts("Failed to set keyboard data format");
		return false;
	}
	//													 DISCL_BACKGROUND | DISCL_EXCLUSIVE	
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(MainWindowHandle, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		puts("Failed to set keyboard cooperative level");
		return false;
	}

	g_pDevKeyboard->Acquire();

	return true;
}

void Keyboard_Finalize(void)
{
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

void Keyboard_Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

bool GetKeyDown(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

bool GetKey(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

bool GetKeyUp(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}


//		GAMEPAD

//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pInput->CreateDevice(lpddi->guidInstance, &g_pGamePad[g_padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���
}

bool GamePad_Initialize()
{

	HRESULT		result;
	int			i;

	g_padCount = 0;
	// �W���C�p�b�h��T��
	g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i < g_padCount; i++)
	{
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = g_pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		//		if ( FAILED(result) )
		//			return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		g_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		g_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		g_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		g_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		g_pGamePad[i]->Acquire();
	}

	return true;
}

void GamePad_Finalize(void)
{
	for (int i = 0; i < GAMEPADMAX; i++) {
		if (g_pGamePad[i])
		{
			g_pGamePad[i]->Unacquire();
			g_pGamePad[i]->Release();
		}
	}
}

DIJOYSTATE2		dijs[GAMEPADMAX];
void GamePad_Update(void)
{
	HRESULT			result;
	int				i;

	for (i = 0; i < g_padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = g_padState[i];
		g_padState[i] = 0x00000000l;	// ������

		result = g_pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = g_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePad[i]->Acquire();
		}

		result = g_pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs[i]);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = g_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePad[i]->Acquire();
		}

		if (dijs[i].rgbButtons[0] & 0x80)	g_padState[i] |= BUTTON_A;
		if (dijs[i].rgbButtons[1] & 0x80)	g_padState[i] |= BUTTON_B;
		if (dijs[i].rgbButtons[2] & 0x80)	g_padState[i] |= BUTTON_C;
		if (dijs[i].rgbButtons[3] & 0x80)	g_padState[i] |= BUTTON_X;
		if (dijs[i].rgbButtons[4] & 0x80)	g_padState[i] |= BUTTON_Y;
		if (dijs[i].rgbButtons[5] & 0x80)	g_padState[i] |= BUTTON_Z;
		if (dijs[i].rgbButtons[6] & 0x80)	g_padState[i] |= BUTTON_L;
		if (dijs[i].rgbButtons[7] & 0x80)	g_padState[i] |= BUTTON_R;
		if (dijs[i].rgbButtons[8] & 0x80)	g_padState[i] |= BUTTON_START;
		if (dijs[i].rgbButtons[9] & 0x80)	g_padState[i] |= BUTTON_M;

		g_padTrigger[i] = ((lastPadState ^ g_padState[i]) & g_padState[i]);
		g_padRelease[i] = ((lastPadState ^ g_padState[i]) & lastPadState);

	}
}

bool GetButton(int padNo, DWORD button) {
	return (button & g_padState[padNo]);
}
bool GetButtonDown(int padNo, DWORD button) {
	return (button & g_padTrigger[padNo]);
}
bool GetButtonUp(int padNo, DWORD button) {
	return (button & g_padRelease[padNo]);
}

float GetAxis(int padNo, Axis axis) {
	switch (axis) {
	case LeftHorizontal:
		return (float)dijs[padNo].lX / RANGE_MAX;
	case LeftVertical:
		return (float)dijs[padNo].lY / RANGE_MAX;
	case RightHorizontal:
		return (float)((dijs[padNo].lZ / 3.27675) - RANGE_MAX) / RANGE_MAX; //minus mod deadzone for stabilization
	case RightVertical:
		return (float)-((dijs[padNo].lRz / 3.27675) - RANGE_MAX) / RANGE_MAX;
	}
}

//		MOUSE

SHORT m_stateOLD[5], m_stateNEW[6];
void MouseInit() {
	mousePosition.x = 0;
	mousePosition.y = 0;

	for (int i = 0; i < 5; i++) {
		m_stateOLD[i] = 0;
		m_stateNEW[i] = 0;
	}
}

void MouseUpdate() {
	GetCursorPos(&mousePosition);
	for (int i = 0; i < 5; i++)m_stateOLD[i] = m_stateNEW[i];
	m_stateNEW[0] = GetAsyncKeyState(VK_LBUTTON);
	m_stateNEW[1] = GetAsyncKeyState(VK_RBUTTON);
	m_stateNEW[2] = GetAsyncKeyState(VK_MBUTTON);
	m_stateNEW[3] = GetAsyncKeyState(VK_XBUTTON1);
	m_stateNEW[4] = GetAsyncKeyState(VK_XBUTTON2);
}

//Return continuous true as long as key is pushed
bool GetMouseButton(int nButton) {
	return (m_stateNEW[nButton] & 0x8000) ? true : false;
}

//Returns true only once the button is pushed
bool GetMouseButtonDown(int nButton) {
	return ((m_stateOLD[nButton] ^ m_stateNEW[nButton]) & m_stateNEW[nButton] & 0x8000) ? true : false;
}

//Returns true only once the button is released
bool GetMouseButtonUp(int nButton) {
	return ((m_stateOLD[nButton] ^ m_stateNEW[nButton]) & m_stateOLD[nButton] & 0x8000) ? true : false;
}

//System functions

bool InputInitialize(HINSTANCE hInstance, HWND hWnd) {
	MainWindowHandle = hWnd;
	InstanceHandle = hInstance;
	InitDirectInput();
	Keyboard_Initialize();
	GamePad_Initialize();
	MouseInit();
	return true;
}

void InputUpdate() {
	Keyboard_Update();
	MouseUpdate();
	GamePad_Update();
}

void InputRelease() {
	Keyboard_Finalize();
	GamePad_Finalize();
	ReleaseDirectInput();
}