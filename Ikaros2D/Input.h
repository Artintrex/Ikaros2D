#pragma once
#include <dinput.h>

//Unused for now
#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)


enum Axis {
	LeftHorizontal,
	LeftVertical,
	RightHorizontal,
	RightVertical
};


#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)
#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット
#define	NUM_KEY_MAX			(256)


bool InputInitialize(HINSTANCE hInstance, HWND hWnd);
void InputUpdate();
void InputRelease();

bool GetKey(int nKey);
bool GetKeyDown(int nKey);
bool GetKeyUp(int nKey);

bool GetButton(int padNo, DWORD button);
bool GetButtonDown(int padNo, DWORD button);
bool GetButtonUp(int padNo, DWORD button);

float GetAxis(int padNo, Axis axis);

bool GetMouseButton(int nButton);
bool GetMouseButtonDown(int nButton);
bool GetMouseButtonUp(int nButton);

extern POINT mousePosition;
extern int mouseScrollDelta;