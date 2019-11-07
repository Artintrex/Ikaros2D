#pragma once
#include "IkarosCore.h"

class MyScript : public MonoBehavior {
public:
	GameObject* test;
	GameObject* camObj;
	GameObject* cam;
	void Awake() {
	//DONT USE YET
		//asset/texture/field000.jpg
	}

	void Start() {
		camObj = new GameObject("MainCamera");
		camObj->AddComponent<Camera>();
		camObj->transform->Translate(Vector3(0, 0, -100));


		test = new GameObject("Tester");
		Texture* tex = Texture::LoadTexture("TestImage", "Assets/Textures/test.jpg");
		Sprite* tsprite = new Sprite();
		
		tsprite->texture = tex;
		//tsprite->SetTexture();

		//Renderer* rend = test->AddComponent<Renderer>();
		//rend->sprite = tsprite;
		
	}

	void Update() {

	}
};