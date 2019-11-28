#pragma once
#include "IkarosCore.h"

class SceneAlpha : public MonoBehavior {
public:
	GameObject* PlayerController1;
	GameObject* PlayerController2;
	GameObject* GroundCollider;
	GameObject* MainCamera;

	Texture* texBG[7];
	Sprite* sprBG[7];

	Texture* texFloor;
	Sprite* sprFloor;

	Texture* texBox;
	Sprite* sprBox;

	Player1* player1;

	void Awake() {

	}

	void Start() {
		MainCamera = new GameObject("MainCamera");
		MainCamera->AddComponent<Camera>();
		MainCamera->transform->Translate(0, 0, -40);

		PlayerController1 = new GameObject("PlayerController1");
		PlayerController2 = new GameObject("PlayerController2");
		player1 = PlayerController1->AddComponent<Player1>();
		PlayerController2->AddComponent<player2>();

		GroundCollider = new GameObject("GroundCollider");
		GroundCollider->transform->position = Vector3(0, -21, 0);
		RigidBody* ground = GroundCollider->AddComponent<RigidBody>();
		ground->AddBoxCollider(Vector2(150, 1));


		texBG[0] = Texture::LoadTexture("clouds1", "Assets/Textures/BG/clouds_1.png");
		texBG[1] = Texture::LoadTexture("clouds2", "Assets/Textures/BG/clouds_2.png");
		texBG[2] = Texture::LoadTexture("clouds3", "Assets/Textures/BG/clouds_3.png");
		texBG[3] = Texture::LoadTexture("clouds4", "Assets/Textures/BG/clouds_4.png");
		texBG[4] = Texture::LoadTexture("rocks1", "Assets/Textures/BG/rocks_1.png");
		texBG[5] = Texture::LoadTexture("rocks2", "Assets/Textures/BG/rocks_2.png");
		texBG[6] = Texture::LoadTexture("sky", "Assets/Textures/BG/sky.png");

		for (int i = 0; i < 7; i++)
		{
			sprBG[i] = new Sprite();
			sprBG[i]->texture = texBG[i];
			sprBG[i]->GenereteSprite();
		}

		GameObject* sky = new GameObject("sky");
		sky->AddComponent<Renderer>()->sprite = sprBG[6];
		sky->transform->position = Vector3(0, 0, 50);
		sky->transform->scale = Vector3(10, 10, 1);

		GameObject* rocks1 = new GameObject("rocks1");
		rocks1->AddComponent<Renderer>()->sprite = sprBG[4];
		rocks1->transform->position = Vector3(0, 0, 40);
		rocks1->transform->scale = Vector3(7.5, 7.5, 1);

		GameObject* rocks2 = new GameObject("rocks2");
		rocks2->AddComponent<Renderer>()->sprite = sprBG[5];
		rocks2->transform->position = Vector3(0, 0, 25);
		rocks2->transform->scale = Vector3(7.5, 7.5, 1);

		GameObject* clouds = new GameObject("clouds");
		clouds->AddComponent<Renderer>()->sprite = sprBG[1];
		clouds->transform->position = Vector3(0, 0, 15);
		clouds->transform->scale = Vector3(4, 4, 1);

		texFloor = Texture::LoadTexture("floor", "Assets/Textures/floor.png");
		sprFloor = new Sprite();
		sprFloor->texture = texFloor;
		sprFloor->GenereteSprite();

		for (float i = 75; i > -75; i -= 3.6) {
			CreateFloor(i, -21);
		}

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
		MainCamera->transform->position.x = player1->player1->transform->position.x;

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
		box->AddComponent<Renderer>()->sprite = sprBox;
		box->transform->position = Vector3(x, y, 0);
		box->transform->scale *= size;

		RigidBody* rb_box = box->AddComponent<RigidBody>();
		rb_box->SetType(b2_dynamicBody);
		rb_box->AddBoxCollider(sprBox->size * size);

		numberofBoxes++;
	}
};