#pragma once
#include "IkarosCore.h"

class Title : public MonoBehavior {
public:
	GameObject* TitleBack;
	GameObject* Option;
	GameObject* Exit;
	GameObject* Editor;
	GameObject* ButtonBackdrop;
	GameObject* MainCamera;
	int BUTTON = 0, BACKDROPPOS = 0;
	int BUTTONS[3] = { -790,-755,-830 };

	void Awake() {

	}

	void Start() {
		MainCamera = new GameObject("MainCamera");
		Camera* cam = MainCamera->AddComponent<Camera>();
		MainCamera->transform->Translate(0, 0, -40);
		cam->orthographic = true;
		cam->orthographicSize = 1;
		cam->SetProjection();


	TitleBack = new GameObject("TiTleBack");
	Option = new GameObject("Option");
	Exit = new GameObject("Exit");
	Editor = new GameObject("Editor");
	ButtonBackdrop = new GameObject("ButtonBackdrop");


	Texture* TitleBackTex = Texture::LoadTexture("TitleBackTex", "assets/Textures/title.jpg");
	Texture* OptionTex = Texture::LoadTexture("OptionTex", "assets/Textures/option.png");
	Texture* ExitTex = Texture::LoadTexture("ExitTex", "assets/Textures/exit.png");
	Texture* EditorTex = Texture::LoadTexture("EditorTex", "assets/Textures/editor.png");
	Texture* ButtonBackdropTex = Texture::LoadTexture("ButtonBackdropTex", "assets/Textures/ButtonBackdrop.png");

	Sprite* TitleBackSpr = new Sprite();
	Sprite* OptionSpr = new Sprite();
	Sprite* ExitSpr = new Sprite();
	Sprite* EditorSpr = new Sprite();
	Sprite* ButtonBackdropSpr = new Sprite();
	TitleBack->AddComponent<Renderer>()->sprite = TitleBackSpr;
	TitleBack->transform->position = Vector3(0, 0, 0.0010);
	TitleBack->transform->Scale(100, 100, 1);

	ButtonBackdrop->AddComponent<Renderer>()->sprite = ButtonBackdropSpr;
	ButtonBackdrop->transform->Scale(100, 100, 1);
	ButtonBackdrop->transform->position = Vector3(-700, -BACKDROPPOS, 0011);

	Option->AddComponent<Renderer>()->sprite = OptionSpr;
	Option->transform->position = Vector3(BUTTONS[0], 0, 0.0012);
	Option->transform->Scale(100, 100, 1);


	Editor->AddComponent<Renderer>()->sprite = EditorSpr;
	Editor->transform->Scale(100, 100, 1);
	Editor->transform->position = Vector3(BUTTONS[1], -100, 0.0012);


	Exit->AddComponent<Renderer>()->sprite = ExitSpr;
	Exit->transform->Scale(100, 100, 1);
	Exit->transform->position = Vector3(BUTTONS[2], -200, 0.0012);

	}

	void Update() {
		if (GetKeyDown(DIK_S))
		{
			BUTTON++;
			if (BUTTON > 2)BUTTON = 0;
			BACKDROPPOS = BUTTON * 100;
		}
		if (GetKeyDown(DIK_W))
		{
			BUTTON--;
			if (BUTTON < 0)BUTTON = 2;
			BACKDROPPOS = BUTTON * 100;
		}
		ButtonBackdrop->transform->position = Vector3(-700, -BACKDROPPOS, 0.0011);


	}
};