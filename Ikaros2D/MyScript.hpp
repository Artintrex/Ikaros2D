#pragma once
#include "GameHeader.h"

class MyScript : public MonoBehavior {
public:
	GameObject* test;
	GameObject* camObj;

	void Awake() {

	}

	void Start() {
		camObj = new GameObject("MainCamera");
		camObj->AddComponent<Camera>();
		camObj->transform->Translate(0, 2, -5000);


		test = new GameObject("Tester");
		Texture* tex = Texture::LoadTexture("TestImage", "Assets/Textures/test.jpg");
		Sprite* tsprite = new Sprite();
		
		tsprite->texture = tex;
		Renderer* rend = test->AddComponent<Renderer>();
		rend->sprite = tsprite;
		tsprite->SetTexture();

		test->transform->Scale(1, 1, 1);

		//if (Keyboard_IsPress(DIK_A)) {
		//	test->transform->Translate(5, 0, 0);
		//	test->transform->Rotate(45, 0, 0);
		//}
	}

	void Update() {
		test->transform->Rotate(1, 0, 0);
	}
};