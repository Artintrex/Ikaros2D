#pragma once
#include "GameHeader.h"

class MyScript : public MonoBehavior {
public:
	GameObject* test;
	GameObject* camObj;
	GameObject* cam;

	float rot;
	void Awake() {
	//DONT USE YET
	}

	void Start() {
		rot = 0;
		camObj = new GameObject("MainCamera");
		camObj->AddComponent<Camera>();
		camObj->transform->Translate(0, 2, -25);


		test = new GameObject("Tester");
		Texture* tex = Texture::LoadTexture("TestImage", "Assets/Textures/test.jpg");
		Sprite* tsprite = new Sprite();
		
		tsprite->texture = tex;
		Renderer* rend = test->AddComponent<Renderer>();
		rend->sprite = tsprite;
		tsprite->SetTexture();

		test->transform->Scale(0.01, 0.01, 0.01);

		//if (Keyboard_IsPress(DIK_A)) {
		//	test->transform->Translate(5, 0, 0);
		//	test->transform->Rotate(45, 0, 0);
		//}
	}

	void Update() {
		rot += Time.DeltaTime * 10;
		test->transform->Rotate(rot, rot, rot);


	}
};