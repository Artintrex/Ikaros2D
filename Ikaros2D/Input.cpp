#include "Input.h"
#include "GameHeader.h"


#define DIRECTINPUT_VERSION (0x0800)

//Gamepad parameters
#define DEADZONE		2500
#define RANGE_MAX		10000
#define RANGE_MIN		-10000


static bool initialize(HINSTANCE hInstance);
static void finalize(void);

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
static int					g_padCount = 0;



bool initialize(HINSTANCE hInstance)
{
	if (g_pInput == NULL) {

		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) {

			return false;
		}
	}

	return true;
}

void finalize(void)
{
	if (g_pInput != NULL) {
		g_pInput->Release();
		g_pInput = NULL;
	}
}

bool Keyboard_Initialize(HINSTANCE hInstance, HWND g_hWnd)
{
	if (!initialize(hInstance)) {

		MessageBox(g_hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(g_hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(g_hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return false;
	}
	//													 DISCL_BACKGROUND | DISCL_EXCLUSIVE	
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(g_hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(g_hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
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

	finalize();
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

//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pInput->CreateDevice(lpddi->guidInstance, &g_pGamePad[g_padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���
}

bool GamePad_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	if (!initialize(hInstance)) {

		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

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

	finalize();
}

void GamePad_Update(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
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

		result = g_pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = g_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if (dijs.lY < 0)					g_padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					g_padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					g_padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)				g_padState[i] |= BUTTON_RIGHT;

		if (dijs.rgbButtons[0] & 0x80)	g_padState[i] |= BUTTON_A;
		if (dijs.rgbButtons[1] & 0x80)	g_padState[i] |= BUTTON_B;
		if (dijs.rgbButtons[2] & 0x80)	g_padState[i] |= BUTTON_C;
		if (dijs.rgbButtons[3] & 0x80)	g_padState[i] |= BUTTON_X;
		if (dijs.rgbButtons[4] & 0x80)	g_padState[i] |= BUTTON_Y;
		if (dijs.rgbButtons[5] & 0x80)	g_padState[i] |= BUTTON_Z;
		if (dijs.rgbButtons[6] & 0x80)	g_padState[i] |= BUTTON_L;
		if (dijs.rgbButtons[7] & 0x80)	g_padState[i] |= BUTTON_R;
		if (dijs.rgbButtons[8] & 0x80)	g_padState[i] |= BUTTON_START;
		if (dijs.rgbButtons[9] & 0x80)	g_padState[i] |= BUTTON_M;

		g_padTrigger[i] = ((lastPadState ^ g_padState[i]) & g_padState[i]);

	}
}

BOOL GamePad_IsPress(int padNo, DWORD button)
{
	return (button & g_padState[padNo]);
}

BOOL GamePad_IsTrigger(int padNo, DWORD button)
{
	return (button & g_padTrigger[padNo]);
}

/* MOUSE    //�K�v���Ȃ�
if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {

}

if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {

}
*/

POINT p;
SHORT m_stateOLD[6], m_stateNEW[6];
MSG* msg;

void MouseInit() {
	p.x = 0;
	p.y = 0;

	for (int i = 0; i < 6; i++) {
		m_stateOLD[i] = 0;
		m_stateNEW[i] = 0;
	}
}

void MouseUpdate() {

	GetCursorPos(&p);
}

//NEED UPDATE mouse wheel need windows messages

//Using virtual key codes 
bool GetMouseButton(int nButton) {
	if (GetAsyncKeyState(nButton) & 0x8000)return true;
	else return false;
}
//Using virtual key codes 
//bool GetMouseButtonDown(int nButton) {
//
//}
//Using virtual key codes 
//bool GetMouseButtonUp(int nButton) {

//}

bool InputInitialize(HINSTANCE hInstance, HWND hWnd, MSG* messages) {
	initialize(hInstance);
	Keyboard_Initialize(hInstance, hWnd);
	GamePad_Initialize(hInstance, hWnd);
	MouseInit();
	msg = messages;

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
	finalize();
}