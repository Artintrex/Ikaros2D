#pragma once
#include "IkarosCore.h"

class SceneAlpha : public MonoBehavior {
public:
	SceneAlpha() {
		type = typeid(*this).name();
		mb_init();
	}
	GameObject* GroundCollider;

	Texture* texBG[7];
	Sprite* sprBG[7];

	Texture* texFloor;
	Sprite* sprFloor;

	Texture* texBox;
	Sprite* sprBox;

	void Awake() {
		texBG[0] = Texture::LoadTexture("clouds1Sprite", "Assets/Textures/BG/clouds_1.png");
		texBG[1] = Texture::LoadTexture("clouds2Sprite", "Assets/Textures/BG/clouds_2.png");
		texBG[2] = Texture::LoadTexture("clouds3Sprite", "Assets/Textures/BG/clouds_3.png");
		texBG[3] = Texture::LoadTexture("clouds4Sprite", "Assets/Textures/BG/clouds_4.png");
		texBG[4] = Texture::LoadTexture("rocks1Sprite", "Assets/Textures/BG/rocks_1.png");
		texBG[5] = Texture::LoadTexture("rocks2Sprite", "Assets/Textures/BG/rocks_2.png");
		texBG[6] = Texture::LoadTexture("skySprite", "Assets/Textures/BG/sky.png");

		for (int i = 0; i < 7; i++)
		{
			sprBG[i] = new Sprite();
			sprBG[i]->texture = texBG[i];
			sprBG[i]->GenereteSprite();
		}
	}

	void Start() {
		GameObject* sky = new GameObject("sky1");
		sky->AddComponent<Renderer>()->sprite = sprBG[6];
		sky->transform->position = Vector3(0, 0, 1000);
		sky->transform->scale = Vector3(150, 150, 1);

		GameObject* sky2 = new GameObject("sky2");
		sky2->AddComponent<Renderer>()->sprite = sprBG[6];
		sky2->transform->position = Vector3(sprBG[6]->size.x * 150 - 5, 0, 999.9);
		sky2->transform->scale = Vector3(150, 150, 1);

		GameObject* sky3 = new GameObject("sky3");
		sky3->AddComponent<Renderer>()->sprite = sprBG[6];
		sky3->transform->position = Vector3(sprBG[6]->size.x * -150 + 5, 0, 999.9);
		sky3->transform->scale = Vector3(150, 150, 1);

		GameObject* Brocks1 = new GameObject("Brocks1");
		Brocks1->AddComponent<Renderer>()->sprite = sprBG[4];
		Brocks1->transform->position = Vector3(0, 0, 600);
		Brocks1->transform->scale = Vector3(75, 75, 1);

		GameObject* Brocks2 = new GameObject("Brocks2");
		Brocks2->AddComponent<Renderer>()->sprite = sprBG[4];
		Brocks2->transform->position = Vector3(sprBG[4]->size.x * 75 - 0.1, 0, 599.9);
		Brocks2->transform->scale = Vector3(75, 75, 1);

		GameObject* Brocks3 = new GameObject("Brocks3");
		Brocks3->AddComponent<Renderer>()->sprite = sprBG[4];
		Brocks3->transform->position = Vector3(sprBG[4]->size.x * -75 + 0.1, 0, 599.9);
		Brocks3->transform->scale = Vector3(75, 75, 1);

		GameObject* Frocks1 = new GameObject("Frocks1");
		Frocks1->AddComponent<Renderer>()->sprite = sprBG[5];
		Frocks1->transform->position = Vector3(0, -25, 200);
		Frocks1->transform->scale = Vector3(30, 30, 1);

		GameObject* Frocks2 = new GameObject("Frocks2");
		Frocks2->AddComponent<Renderer>()->sprite = sprBG[5];
		Frocks2->transform->position = Vector3(sprBG[4]->size.x * 30 - 0.1, -25, 199.9);
		Frocks2->transform->scale = Vector3(30, 30, 1);

		GameObject* Frocks3 = new GameObject("Frocks3");
		Frocks3->AddComponent<Renderer>()->sprite = sprBG[5];
		Frocks3->transform->position = Vector3(sprBG[4]->size.x * -30 + 0.1, -25, 199.9);
		Frocks3->transform->scale = Vector3(30, 30, 1);

		GameObject* Bclouds1 = new GameObject("Bclouds1");
		Bclouds1->AddComponent<Renderer>()->sprite = sprBG[0];
		Bclouds1->transform->position = Vector3(0, 0, 350);
		Bclouds1->transform->scale = Vector3(40, 40, 1);

		GameObject* Bclouds2 = new GameObject("Bclouds2");
		Bclouds2->AddComponent<Renderer>()->sprite = sprBG[0];
		Bclouds2->transform->position = Vector3(sprBG[0]->size.x * 40 - 0.1, 0, 349.9);
		Bclouds2->transform->scale = Vector3(40, 40, 1);

		GameObject* Bclouds3 = new GameObject("Bclouds3");
		Bclouds3->AddComponent<Renderer>()->sprite = sprBG[0];
		Bclouds3->transform->position = Vector3(sprBG[0]->size.x * -40 + 0.1, 0, 349.9);
		Bclouds3->transform->scale = Vector3(40, 40, 1);

		GameObject* Fclouds1 = new GameObject("Fclouds1");
		Fclouds1->AddComponent<Renderer>()->sprite = sprBG[1];
		Fclouds1->transform->position = Vector3(200, -10, 250);
		Fclouds1->transform->scale = Vector3(15, 15, 1);

		GameObject* Fclouds2 = new GameObject("Fclouds2");
		Fclouds2->AddComponent<Renderer>()->sprite = sprBG[2];
		Fclouds2->transform->position = Vector3(-200, -10, 150);
		Fclouds2->transform->scale = Vector3(15, 15, 1);

		GameObject* Fclouds3 = new GameObject("Fclouds3");
		Fclouds3->AddComponent<Renderer>()->sprite = sprBG[3];
		Fclouds3->transform->position = Vector3(-400, -50, 150);
		Fclouds3->transform->scale = Vector3(15, 15, 1);

		texFloor = Texture::LoadTexture("floor", "Assets/Textures/floor.png");
		sprFloor = new Sprite();
		sprFloor->texture = texFloor;
		sprFloor->GenereteSprite();

		for (float i = 150; i > -150; i -= 3.6) {
			CreateFloor(i, -50);
		}

		GroundCollider = new GameObject("GroundCollider");
		GroundCollider->tag = "Ground";
		GroundCollider->transform->position = Vector3(0, -50, 0);
		RigidBody* ground = GroundCollider->AddComponent<RigidBody>();
		ground->AddBoxCollider(Vector2(250, 1));

		GameObject* WallE = new GameObject("EastWall");
		WallE->tag = "Wall";
		WallE->transform->position = Vector3(125, -50, 0);
		RigidBody* rbWE = WallE->AddComponent<RigidBody>();
		rbWE->AddBoxCollider(Vector2(1, 500));

		GameObject* WallW = new GameObject("WestWalls");
		WallW->tag = "Wall";
		WallW->transform->position = Vector3(-125, -50, 0);
		RigidBody* rbWW = WallW->AddComponent<RigidBody>();
		rbWW->AddBoxCollider(Vector2(1, 500));



		texBox = Texture::LoadTexture("Box", "Assets/Textures/crate.png");
		sprBox = new Sprite();
		sprBox->texture = texBox;
		sprBox->GenereteSprite();

		CreateBox(0, 0);

		CreateBox(20, 0, 0.5);
		CreateBox(20, 10, 0.5);
		CreateBox(20, 20, 0.5);
		CreateBox(20, 30, 0.5);
		CreateBox(20, 40, 0.5);

		CreateBox(-20, 40, 0.5);
		CreateBox(-20, 30, 0.5);
		CreateBox(-20, 20, 0.5);
		CreateBox(-20, 10, 0.5);
		CreateBox(-20, 10, 0.5);
		CreateBox(-20, 10, 0.5);
		
	}

	void Update() {
		if (GetKeyDown(DIK_F)) {
			CreateBox(0, 0, 0.5);
		}
	}

	int numberofFloor = 0;
	void CreateFloor(float x, float y) {
		GameObject* floorblock = new GameObject((std::string)"floor" + std::to_string(numberofFloor));
		floorblock->AddComponent<Renderer>()->sprite = sprFloor;
		floorblock->transform->position = Vector3(x, y, 0);

		numberofFloor++;
	}

	int numberofBoxes = 0;
	void CreateBox(float x, float y, float size = 1) {
		GameObject* box = new GameObject("box" + std::to_string(numberofBoxes));
		box->tag = "Box";
		box->AddComponent<Renderer>()->sprite = sprBox;
		box->transform->position = Vector3(x, y, 0);
		box->transform->scale *= size;

		RigidBody* rb_box = box->AddComponent<RigidBody>();
		rb_box->SetType(b2_dynamicBody);
		rb_box->AddBoxCollider(sprBox->size * size);

		numberofBoxes++;
	}
};