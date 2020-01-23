#pragma once
#include "IkarosCore.h"



class Template : public MonoBehavior {
public:
	Template() {
		type = typeid(*this).name();
		mb_init();
	}
	
	std::vector <Sprite*> ThunderSprite;
	std::vector<Sprite*> Effect;
	Renderer* ThunderRenderer;
	Renderer* EffectRenderer;
	Camera* mainCamera;

	//—‹‘Ò‹@ŽžŠÔ
	float ThunderTime = 60;
	//—‹‚ð~‚ç‚·ŽžŠÔ
	float GodsThunderTime = 30;
	RigidBody* RigidThunder;
	RigidBody* RigidEffect;

	void Awake() {
		GameObject* zeus = GameObject::Find("zeus");
	}

	void Start() {
		RigidThunder->AddForce(Vector2(0, 10), VelocityChange);

		RigidThunder->AddBoxCollider(Vector2(ThunderSprite[0]->size.x * parent->transform->scale.x * 0.5f,
			ThunderSprite[0]->size.y * parent->transform->scale.y * 1.5));
		ThunderRenderer = parent->AddComponent<Renderer>();
		EffectRenderer = parent->AddComponent<Renderer>();
		mainCamera = Camera::main;
		transform->position.x = mainCamera->transform->position.x + (rand() % 256) - 128;
		transform->position.y = mainCamera->transform->position.y + 900;

	}

	void Update() {


		ThunderTime -= Time.DeltaTime;
		GodsThunderTime -= Time.DeltaTime;

	}
};

void OnCollisionEnter(Collision collider) {

	if (collider.parent->tag == "Ground") {
		
		//effect

	}

}