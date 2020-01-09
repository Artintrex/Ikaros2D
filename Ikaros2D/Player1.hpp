#pragma once
#include "IkarosCore.h"
#include "Javelin.hpp"

class Player1 : public MonoBehavior {
public:
	Player1() {
		type = typeid(*this).name();
		mb_init();
	}
	RigidBody* rigidbody;
	Renderer* renderer;

	Sprite* RunningSprite[9];

	void Awake() {

	}

	void Start() {
		for (int i = 0; i < 9; i++)
		{
			std::string textureName = "player_running" + std::to_string(i + 1);

			RunningSprite[i] = new Sprite(textureName + "Sprite");
			RunningSprite[i]->doubleSided = true;
			RunningSprite[i]->GenereteSprite(textureName);
		}
		
		renderer = parent->AddComponent<Renderer>();

		parent->transform->Scale(0.6, 0.6, 0.6);
		renderer->sprite = RunningSprite[0];

		rigidbody = parent->AddComponent<RigidBody>();
		rigidbody->SetType(b2_dynamicBody);
		rigidbody->rigidbody->SetGravityScale(5.0f);
		rigidbody->rigidbody->SetFixedRotation(true);
		rigidbody->AddBoxCollider(Vector2(RunningSprite[0]->size.x - 2, RunningSprite[0]->size.y - 0.4) * 0.6f);
	}

	int cnt = 0, direction = 1;
	float Timer = 0.5;
	bool JumpFlag = true;
	void Update() {
		renderer->sprite = RunningSprite[cnt];
		if (Timer < 0) {
			cnt++;
			Timer = 0.5;
		}
		Timer -= Time.DeltaTime * abs(rigidbody->velocity.x);
		if (cnt > 8) cnt = 0;

		if(abs(rigidbody->velocity.y) > 0.1f)JumpFlag = false;

		if (GetKey(DIK_A)) {
			if (JumpFlag) {
				rigidbody->AddForce(Vector2(-500, 0), Force);
			}
			transform->scale = Vector3(-0.6, 0.6, 0.6);
			direction = -1;
		}
		if (GetKey(DIK_D)) {
			if (JumpFlag) {
				rigidbody->AddForce(Vector2(500, 0), Force);
			}
			transform->scale = Vector3(0.6, 0.6, 0.6);
			direction = 1;
		}
		if (GetKeyDown(DIK_SPACE) && JumpFlag) {
			rigidbody->AddForce(Vector2(0, 600), Impulse);
		}
		if (GetKeyDown(DIK_B)) {
			GameObject* jav = new GameObject("Javelin");
			jav->transform->position = Vector3(transform->position.x + (direction * 3), 
												transform->position.y, 
												transform->position.z);

			Javelin* gjav = jav->AddComponent<Javelin>();
			gjav->direction = direction;
		}
	}

	void OnCollision(Collision collider) {
		if (rigidbody->velocity.y <= 0) {
			JumpFlag = true;
		}
	}
};