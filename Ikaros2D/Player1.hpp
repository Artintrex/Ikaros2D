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

	std::vector <Sprite*> RunningSprite;
	std::vector <Sprite*> RunningSpriteWithSword;
	std::vector <Sprite*> AttackSprite;
	std::vector <Sprite*> IdleSprite;

	void Awake() {

	}

	Vector3 scale = Vector3(1.5, 1.5, 1);
	void Start() {
		renderer = parent->AddComponent<Renderer>();

		parent->transform->scale = scale;
		renderer->sprite = RunningSprite[0];

		rigidbody = parent->AddComponent<RigidBody>();
		rigidbody->SetType(b2_dynamicBody);
		rigidbody->rigidbody->SetGravityScale(5.0f);
		rigidbody->rigidbody->SetFixedRotation(true);
		rigidbody->AddBoxCollider(Vector2(RunningSprite[0]->size.x * parent->transform->scale.x * 0.5f,
			RunningSprite[0]->size.y * parent->transform->scale.y * 0.8));

		rigidbody->Translate(20, 0, 0);
	}

	int cnt = 0, direction = 1;
	float Timer = 0.5;
	bool JumpFlag = true;

	int ActiveItem = 0;
	int JavelinCount = 0;

	int AtkCnt = 0;
	bool isAttacking = false;
	float AttackDelay = 0.2;
	float AttackTimer = AttackDelay;

	void Update() {
		if (Timer < 0) {
			cnt++;
			Timer = 0.5;
		}
		Timer -= Time.DeltaTime * abs(rigidbody->velocity.x);
		if (cnt >= RunningSprite.size()) cnt = 0;

		if (ActiveItem == 0) {
			renderer->sprite = RunningSprite[cnt];
		}else renderer->sprite = RunningSpriteWithSword[cnt];


		if(abs(rigidbody->velocity.y) > 0.1f)JumpFlag = false;

		if (GetKey(DIK_A)) {
			if (JumpFlag) {
				rigidbody->AddForce(Vector2(-300, 0), Force);
			}
			else rigidbody->AddForce(Vector2(-50, 0), Force);

			transform->scale.x = -scale.x;
			direction = -1;
		}
		if (GetKey(DIK_D)) {
			if (JumpFlag) {
				rigidbody->AddForce(Vector2(300, 0), Force);
			}
			else rigidbody->AddForce(Vector2(50, 0), Force);

			transform->scale.x = scale.x;
			direction = 1;
		}
		if (GetKeyDown(DIK_SPACE) && JumpFlag) {
			rigidbody->AddForce(Vector2(0, 400), Impulse);
		}
		if (GetKeyDown(DIK_LCONTROL) && JavelinCount > 0) {
			GameObject* jav = new GameObject("Javelin");
			jav->transform->position = Vector3(transform->position.x + (direction * 3), 
												transform->position.y, 
												transform->position.z);

			Javelin* gjav = jav->AddComponent<Javelin>();
			gjav->direction = direction;

			JavelinCount--;
		}

		AttackTimer -= Time.DeltaTime;

		if (GetKey(DIK_LSHIFT)) {
			if (isAttacking == false) {
				AttackTimer = AttackDelay; AtkCnt = 0;
			}
			isAttacking = true;
		}

		if (AttackTimer < 0) {
			AtkCnt++;
			AttackTimer = AttackDelay;
		}

		if (AtkCnt >= AttackSprite.size()) {
			isAttacking = false;
			AtkCnt = 0;
		}

		if (isAttacking == true) {
			renderer->sprite = AttackSprite[AtkCnt];
		}
	}

	void OnCollision(Collision collider) {
		if (rigidbody->velocity.y <= 0) {
			JumpFlag = true;
		}

		if (collider.parent->tag == "Box") {
			if (GetKeyDown(DIK_F)) {
				switch (collider.parent->GetComponent<Box>()->TypeOfBox) {
				case 0:
					ActiveItem = gSword;
				case 1:
					JavelinCount += 2;
				}

				Destroy(collider.parent);
			}
		}
	}
};