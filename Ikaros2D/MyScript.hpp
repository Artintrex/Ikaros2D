#pragma once
#include "IkarosCore.h"

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
		camObj->transform->Translate(Vector3(0, 2, -25));


		test = new GameObject("Tester");
		Texture* tex = Texture::LoadTexture("TestImage", "Assets/Textures/test.jpg");
		Sprite* tsprite = new Sprite();
		
		tsprite->texture = tex;
		Renderer* rend = test->AddComponent<Renderer>();
		rend->sprite = tsprite;
		tsprite->SetTexture();

		test->transform->Scale(Vector3(0.01, 0.01, 0.01));
	}

	void Update() {
		rot += Time.DeltaTime;
		test->transform->Rotate(Vector3(rot, rot, rot));
	}
};