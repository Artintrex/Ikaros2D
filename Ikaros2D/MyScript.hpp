#pragma once
#include "GameHeader.h"

class MyScript : public MonoBehavior {
public:
	GameObject* test;
	GameObject* testGround;

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
		camObj->transform->Translate(0, 2, -50);
		cam->rect.W = 1;
		cam->rect.H = 1;
		cam->SetProjection();
		/*
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
		*/

		test = new GameObject("Tester");
		Texture* tex = Texture::LoadTexture("TestImage", "Assets/Textures/test.jpg");
		Sprite* tsprite = new Sprite();
		tsprite->texture = tex;
		tsprite->GenereteSprite();

		Renderer* rend = test->AddComponent<Renderer>();
		rend->sprite = tsprite;

		RigidBody* testBody = test->AddComponent<RigidBody>();
		testBody->rigidbody->SetType(b2_dynamicBody);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f, 1.0f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		testBody->rigidbody->CreateFixture(&fixtureDef);

		test->transform->Scale(1, 1, 1);
		test->transform->position = Vector3(0, 10, 0);

		testGround = new GameObject("Ground");
		Renderer* groundrenderer = testGround->AddComponent<Renderer>();
		groundrenderer->sprite = tsprite;
		testGround->transform->position = Vector3(0, -20, 0);
		testGround->transform->scale = Vector3(5,1,1);
		RigidBody* TestGroundBody = testGround->AddComponent<RigidBody>();

		b2PolygonShape groundBox;
		groundBox.SetAsBox(100.0f, 0.1f);

		TestGroundBody->rigidbody->CreateFixture(&groundBox, 0.0f);

		//b2FixtureDef fixturedef;
		//fixturedef.shape = 

		//if (Keyboard_IsPress(DIK_A)) {
		//	test->transform->Translate(5, 0, 0);
		//	test->transform->Rotate(45, 0, 0);
		//}
	}

	void Update() {
		//test->transform->Rotate(1, 0.5, 0.5);
		test->transform->position.x = test->GetComponent<RigidBody>()->rigidbody->GetPosition().x;
		test->transform->position.y = test->GetComponent<RigidBody>()->rigidbody->GetPosition().y;

		std::cout << test->transform->position.x << ", " << test->transform->position.y << std::endl;
	}
};