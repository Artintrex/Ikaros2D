#pragma once
#include "IkarosCore.h"

class Title : public MonoBehavior {
public:
	GameObject* TitleBack;
	GameObject* Option;
	GameObject* Exit;
	GameObject* Editor;
	GameObject* ButtonBackdrop;

	void Awake() {

	}

	void Start() {
	TitleBack = new GameObject("TiTleBack");
	Option = new GameObject("Option");
	Exit = new GameObject("Exit");
	Editor = new GameObject("Editor");
	ButtonBackdrop = new GameObject("ButtonBackdrop");


	Texture* TitleBackTex = Texture::LoadTexture("TitleBackTex", "assets/Textures/title.jpg");
	Texture* OptionTex = Texture::LoadTexture("OptionTex", "assets/Textures/optien.png");
	Texture* ExitTex = Texture::LoadTexture("ExitTex", "assets/Textures/exit.png");
	Texture* EditorTex = Texture::LoadTexture("EditorTex", "assets/Textures/editor.png");
	Texture* ButtonBackdropTex = Texture::LoadTexture("ButtonBackdropTex", "assets/Textures/ButtonBackdrop.png");

	Sprite* TitleBackSpr = new Sprite();
	Sprite* OptionSpr = new Sprite();
	Sprite* ExitSpr = new Sprite();
	Sprite* EditorSpr = new Sprite();
	Sprite* ButtonBackdropSpr = new Sprite();

	TitleBackSpr->texture = TitleBackTex;
	OptionSpr->texture = OptionTex;
	ExitSpr->texture = ExitTex;
	EditorSpr->texture = EditorTex;
	ButtonBackdropSpr->texture = ButtonBackdropTex;

	TitleBackSpr->GenereteSprite();
	OptionSpr->GenereteSprite();
	ExitSpr->GenereteSprite();
	EditorSpr->GenereteSprite();
	ButtonBackdropSpr->GenereteSprite();

	TitleBack->transform->Scale(0.75,0.75,0.75);
	Option->transform->Scale(0.5, 0.5, 0.5);
	Exit->transform->Scale(0.5, 0.5, 0.5);
	Editor->transform->Scale(0.5, 0.5, 0.5);
	ButtonBackdrop->transform->Scale(0.5, 0.5, 0.5);

	}

	void Update() {

	}
};