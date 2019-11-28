#pragma once
#include "IkarosCore.h"

class Javelin : public MonoBehavior {
public:
	GameObject* spear;

	Javelin(Vector2 pos, int direction, Sprite* sprSpear) {
		spear = new GameObject("spear");
		spear->AddComponent<Renderer>()->sprite = sprSpear;
		spear->transform->position = Vector3(pos.x, pos.y, 0);
		spear->transform->Rotate(0, 0, 90);
		if (direction == -1)spear->transform->position.x -= 2;
		spear->transform->Scale(3, 3, 1);

		RigidBody* rb_spear = spear->AddComponent<RigidBody>();
		rb_spear->SetType(b2_dynamicBody);
		rb_spear->AddBoxCollider(sprSpear->size * 3.0f);
		rb_spear->Rotate(Vector3(0, 0, -90 * direction));
		rb_spear->AddForce(Vector2(50 * direction, 0), VelocityChange);
	}
	void Awake() {

	}

	void Start() {

	}

	float Timer = 25;
	void Update() {
		Timer -= Time.DeltaTime;

		if (Timer < 0) {
			delete spear;  
			delete this;
		}
	}
};