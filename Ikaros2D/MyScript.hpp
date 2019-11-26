#pragma once
#include "GameHeader.h"

class MyScript : public MonoBehavior {
public:
	GameObject* test;
	GameObject* testGround;

	GameObject* camObj;

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


		/*
		camObj2 = new GameObject("Camera2");
		cam = camObj2->AddComponent<Camera>();
		camObj2->transform->Translate(0, 2, -5000);
		cam->rect.X = 0.5;
		cam->rect.W = 0.5;
		cam->rect.H = 0.5;
		cam->SetProjection();
		*/

		test = new GameObject("Tester");
		test->transform->position = Vector3(0, 20, 0);

		Texture* tex = Texture::LoadTexture("TestImage", "Assets/Textures/test.jpg");
		Sprite* tsprite = new Sprite(tex);

		Renderer* rend = test->AddComponent<Renderer>();
		rend->sprite = tsprite;


		RigidBody* testBody = test->AddComponent<RigidBody>();
		testBody->rigidbody->SetTransform(b2Vec2(0, 20), 0);
		testBody->rigidbody->SetType(b2_dynamicBody);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(2.0f, 1.0f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		testBody->rigidbody->CreateFixture(&fixtureDef);

		testGround = new GameObject("Ground");
		Renderer* groundrenderer = testGround->AddComponent<Renderer>();
		groundrenderer->sprite = tsprite;
		testGround->transform->position = Vector3(0, -10, 0);
		testGround->transform->scale = Vector3(5,1,1);
		RigidBody* TestGroundBody = testGround->AddComponent<RigidBody>();
		TestGroundBody->rigidbody->SetTransform(b2Vec2(0, -10), 0);
		b2PolygonShape groundBox;
		groundBox.SetAsBox(30.0f, 6);

		TestGroundBody->rigidbody->CreateFixture(&groundBox, 0.0f);
	}

	void Update() {
		//test->transform->Rotate(1, 0.5, 0.5);
		test->transform->position.x = test->GetComponent<RigidBody>()->rigidbody->GetPosition().x;
		test->transform->position.y = test->GetComponent<RigidBody>()->rigidbody->GetPosition().y;
		test->transform->rotation.z = test->GetComponent<RigidBody>()->rigidbody->GetAngle();


		//std::cout << test->transform->position.x << ", " << test->transform->position.y << std::endl;

		if (GetKeyDown(DIK_SPACE)) {
			test->GetComponent<RigidBody>()->rigidbody->ApplyLinearImpulse(b2Vec2(0, 50),
				test->GetComponent<RigidBody>()->rigidbody->GetPosition(), true);
		}

		if (GetKey(DIK_A)) {
			test->GetComponent<RigidBody>()->rigidbody->ApplyLinearImpulse(b2Vec2(-1, 0),
				test->GetComponent<RigidBody>()->rigidbody->GetPosition(), true);
		}
		if (GetKey(DIK_D)) {
			test->GetComponent<RigidBody>()->rigidbody->ApplyLinearImpulse(b2Vec2(1, 0),
				test->GetComponent<RigidBody>()->rigidbody->GetPosition(), true);
		}
	}
};