#pragma once
#include "GameHeader.h"

class MyScript : public MonoBehavior {
public:
	GameObject* test;
	GameObject* camObj;
	GameObject* camObj2;
	GameObject* camObj3;
	GameObject* camObj4;

	void Awake() {

	}

	void Start() {

		std::cout << "Factory Register" << "\n";
		for (auto it = Component::factories.cbegin(); it != Component::factories.cend(); ++it)
		{
			std::cout << it->first << " " << it->second << "\n";
		}

		Camera* cam;

		camObj = new GameObject("MainCamera");
		cam = camObj->AddComponent<Camera>();
		camObj->transform->Translate(0, 2, -5000);
		cam->rect.W = 0.5;
		cam->rect.H = 0.5;
		cam->SetProjection();

		camObj2 = new GameObject("Camera2");
		cam = camObj2->AddComponent<Camera>();
		camObj2->transform->Translate(0, 2, -5000);
		cam->rect.X = 0.5;
		cam->rect.W = 0.5;
		cam->rect.H = 0.5;
		cam->SetProjection();

		camObj3 = new GameObject("Camera3");
		cam = camObj3->AddComponent<Camera>();
		camObj3->transform->Translate(0, 2, -5000);
		cam->rect.Y = 0.5;
		cam->rect.W = 0.5;
		cam->rect.H = 0.5;
		cam->SetProjection();

		camObj4 = new GameObject("Camera4");
		cam = camObj4->AddComponent<Camera>();
		camObj4->transform->Translate(0, 2, -5000);
		cam->rect.X = 0.5;
		cam->rect.Y = 0.5;
		cam->rect.W = 0.5;
		cam->rect.H = 0.5;
		cam->SetProjection();


		test = new GameObject("Tester");
		Texture* tex = Texture::LoadTexture("TestImage", "Assets/Textures/test.jpg");
		Sprite* tsprite = new Sprite();
		
		tsprite->texture = tex;
		Renderer* rend = test->AddComponent<Renderer>();
		rend->sprite = tsprite;
		tsprite->GenereteSprite();

		test->transform->Scale(1, 1, 1);

		//if (Keyboard_IsPress(DIK_A)) {
		//	test->transform->Translate(5, 0, 0);
		//	test->transform->Rotate(45, 0, 0);
		//}
	}

	void Update() {
		test->transform->Rotate(1, 0.5, 0.5);
	}
};