#pragma once
#include "IkarosCore.h"

class Javelin : public MonoBehavior {
public:
	Javelin() {
		type = typeid(*this).name();
		mb_init();
	}

	Sprite* SpearSprite;
	RigidBody* rb_spear;

	void Awake() {
		Texture::LoadTexture("SpearTexture", "Assets/Textures/javelin.png");
	}

	int direction = 1;
	float speed = 50;

	void Start() {
		SpearSprite = new Sprite("JavelinSprite");
		SpearSprite->GenereteSprite("SpearTexture");

		Renderer* rend = parent->AddComponent<Renderer>();
		rend->sprite = SpearSprite;
		parent->transform->Rotate(0, 0, 90);
		parent->transform->Scale(3, 3, 1);

		rb_spear = parent->AddComponent<RigidBody>();
		rb_spear->SetType(b2_dynamicBody);
		rb_spear->AddBoxCollider(SpearSprite->size * 3.0f);

		rb_spear->Rotate(0, 0, -90 * direction);
		rb_spear->AddForce(Vector2(speed * direction, 0), VelocityChange);
	}

	float Timer = 3;
	void Update() {
		Timer -= Time.DeltaTime;

		if (Timer < 0) {
			delete parent;
		}
	}
};