#pragma once
#include "IkarosCore.h"

class Zeus : public MonoBehavior {
public:
	Zeus() {
		type = typeid(*this).name();
		mb_init();
	}

	float AppearTime = 20;
	Sprite* zeus;

	bool isHidden = true;
	bool isUp = false;
	Vector3 HiddenPosition = Vector3(50, -450, 900);
	Vector3 ElevatedPosition = Vector3(50, 80, 900);

	float Speed = 15;
	Renderer* zeusRenderer;

	void Awake() {
		zeus = new Sprite("ZeusSprite", Texture::LoadTexture("ZeusTexture", "Assets/Textures/zeus.png"));
		zeusRenderer = parent->AddComponent<Renderer>();
		zeusRenderer->sprite = zeus;

		transform->position = HiddenPosition;
		transform->scale = Vector3(100, 100, 1);
	}

	void Start() {

	}

	float Timer = AppearTime;
	void Update() {
		Timer -= Time.DeltaTime;
		if (Timer < 0) {
			isHidden = false;
		}

		if (!isHidden) {
			if (transform->position.y < ElevatedPosition.y) {
				transform->position.y += Speed * Time.DeltaTime;
			}
			else isUp = true;
		}

	}
};