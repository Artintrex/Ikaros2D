#pragma once
#include "IkarosCore.h"

class Result : public MonoBehavior {
public:

	GameObject* MainCamera;
	Sprite* Background;
	void Awake() {
		Background = new Sprite("Background", Texture::LoadTexture("BackgroundTexture", "assets/Textures/context.jpg"));
		GameObject* BG = new GameObject("BackgroundGO");
		BG->AddComponent<Renderer>()->sprite = Background;
		BG->transform->scale = Vector3(100, 100, 100);
	}

	void Start() {
		MainCamera = new GameObject("MainCamera");
		Camera* cam = MainCamera->AddComponent<Camera>();
		MainCamera->transform->Translate(0, 0, -40);
		cam->orthographic = true;
		cam->orthographicSize = 1;
		cam->SetProjection();
	}

	void Update() {
		if (GetKeyDown(DIK_RETURN)) {
			SceneManager::LoadScene(0);
		}
	}
};