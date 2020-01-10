#pragma once
#include "IkarosCore.h"

enum ItemType {
	gNoItem,
	gSword,
	gJavelin,

	NUMBEROFITEMS
};

#define BOX_MAX 2

class Box : public MonoBehavior {
public:
	Box() {
		type = typeid(*this).name();
		mb_init();
	}
	
	static Sprite* BoxSprite[BOX_MAX];
	int TypeOfBox;
	Renderer* BoxRenderer;
	RigidBody* BoxRigidbody;
	Camera* mainCamera;

	void Awake() {
		BoxSprite[0] = new Sprite("BoxSprite1", Texture::LoadTexture("BoxTexture1", "Assets/Textures/crate.png"));
		BoxSprite[1] = new Sprite("BoxSprite2", Texture::LoadTexture("BoxTexture2", "Assets/Textures/crate.png"));
	}

	void Start() {
		BoxRenderer = parent->AddComponent<Renderer>();
	    TypeOfBox = rand() % BOX_MAX;
		BoxRenderer->sprite = BoxSprite[TypeOfBox];

		mainCamera = Camera::main;

		transform->position.x = mainCamera->transform->position.x + (rand() % 256)-128;
		transform->position.y = mainCamera->transform->position.y + 600;

		transform->scale = Vector3(0.5, 0.5, 1);
		
		BoxRigidbody = parent->AddComponent<RigidBody>();
		BoxRigidbody->SetType(b2_dynamicBody);
		BoxRigidbody->AddBoxCollider(BoxSprite[TypeOfBox]->size * transform->scale.x);
	}

	void Update() {

	}
};

class BoxSpawner : public MonoBehavior {
public:
	BoxSpawner() {
		type = typeid(*this).name();
		mb_init();
	}

	float BoxTime = 5, timer;
	void Awake() {

	}

	void Start() {
		timer = BoxTime;
	}

	void Update() {

		if (timer <= 0)
		{
			timer = 5;
			CreateBox();
		}


		timer -= Time.DeltaTime;
	}

	int numberOfBoxes = 0;
	void CreateBox()
	{
		GameObject* box = new GameObject("Box" + std::to_string(numberOfBoxes));
		box->tag = "Box";
		box->AddComponent<Box>();
		numberOfBoxes++;
	}
};