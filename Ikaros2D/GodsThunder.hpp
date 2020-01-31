#pragma once
#include "IkarosCore.h"



class Thunderbolt : public MonoBehavior {
public:
	Thunderbolt() {
		type = typeid(*this).name();
		mb_init();
	}

	std::vector <Sprite*> ThunderSprite;
	std::vector<Sprite*> ThunderEffect;
	Renderer* ThunderRenderer;
	Renderer* EffectRenderer;
	Camera* mainCamera;
	Sprite* Thundersprite;
	Texture* ThundrTexture;

	//—‹‘Ò‹@ŽžŠÔ
	float ThunderTime = 60;
	
	//—‹‚ð~‚ç‚·ŽžŠÔ
	float GodsThunderTime = 30;
	
	RigidBody* RigidThunder;
	RigidBody* RigidEffect;

	float y;

	void Awake() {

	}

	void Start() {
		
		transform->position = mainCamera->ScreenToWorldPoint(Vector3(0, 0, transform->position.z));

		RigidThunder->AddForce(Vector2(0, 10), VelocityChange);

		mainCamera = Camera::main;

		Camera::ScreenToWorldPoint;

		RigidThunder->AddBoxCollider(Vector2(ThunderSprite[0]->size.x * parent->transform->scale.x, 0.1f));

		RigidThunder = parent->AddComponent<RigidBody>();

		ThunderRenderer = parent->AddComponent<Renderer>();
		EffectRenderer = parent->AddComponent<Renderer>();

		Thundersprite->texture = ThundrTexture;

		transform->position.x = mainCamera->transform->position.x + (rand() % 350) - 128;
		transform->position.y = transform->position.y + 300;
		y = transform->position.y;

	}

	void Update() {

		if (transform->position.y < y - ThunderSprite.size.y)
		{
			ThunderSprite[0] = new Sprite("Thunder");
		}


		Thundersprite->GenereteSprite();


		ThunderTime -= Time.DeltaTime;
		GodsThunderTime -= Time.DeltaTime;

	}

	void OnCollisionEnter(Collision collider) {

		if (collider.parent->tag == "Ground") {


			//effect
			//ThunderEffect[0] = new Sprite("ThunderEffect");

		}

	}
};