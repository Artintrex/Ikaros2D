#pragma once
#include "IkarosCore.h"

class GameManager : public MonoBehavior {
public:
	GameManager() {
		type = typeid(*this).name();
		mb_init();
	}

	GameObject* MainCamera;
	Camera* MainCameraComponent;

	PlayerController* gPlayerController;
	BoxSpawner* gBoxSpawner;

	GameObject* gZeus;

	void Awake() {
		parent->AddComponent<SceneAlpha>();
		gPlayerController = parent->AddComponent<PlayerController>();
		gBoxSpawner = parent->AddComponent<BoxSpawner>();

		MainCamera = new GameObject("MainCamera");
		MainCameraComponent = MainCamera->AddComponent<Camera>();
		MainCamera->transform->Translate(0, 0, -40);

		gZeus = new GameObject("Zeus");
		gZeus->AddComponent<Zeus>();
	}

	void Start() {
		parent->AddComponent<Debug>();
	}

	float Timer = 120;
	void Update() {

		//Camera Animation Calculation
		float xP1 = gPlayerController->player1->transform->position.x;
		float xP2 = gPlayerController->player2->transform->position.x;

		float dxP1 = xP1 - MainCamera->transform->position.x;
		float dxP2 = xP2 - MainCamera->transform->position.x;

		float Xavg = (xP1 + xP2) / 2;
		float Yavg = (gPlayerController->player1->transform->position.y + gPlayerController->player2->transform->position.y) / 2;

		float speed = abs(MainCamera->transform->position.x - Xavg);
		float speedY = abs(MainCamera->transform->position.y - Yavg);

		if (MainCamera->transform->position.x > Xavg + 0.2f) {
			MainCamera->transform->position.x -= speed * Time.DeltaTime;
		}
		else if (MainCamera->transform->position.x < Xavg - 0.2f) {
			MainCamera->transform->position.x += speed * Time.DeltaTime;
		}

		if (MainCamera->transform->position.y > Yavg + 15.0f) {
			MainCamera->transform->position.y -= speedY * Time.DeltaTime;
		}
		else if (MainCamera->transform->position.y < Yavg + 5.0f) {
			MainCamera->transform->position.y += speedY * Time.DeltaTime;
		}

		Vector3 lmax = MainCameraComponent->ScreenToWorldPoint(Vector3(0, 0, 0));
		Vector3 Rmax = MainCameraComponent->ScreenToWorldPoint(Vector3(MainCameraComponent->pixelWidth, 0, 0));

		float plmax, pRmax;

		if (xP1 < xP2) {
			plmax = xP1;
			pRmax = xP2;
		}
		else {
			plmax = xP2;
			pRmax = xP1;
		}

		if (plmax < lmax.x + 3 || pRmax > Rmax.x - 3) {
			MainCamera->transform->position.z *= 1.0007f;
		}
		else if (plmax > lmax.x + 15 && pRmax < Rmax.x - 15) {
			MainCamera->transform->position.z *= 0.9993f;
		}

		if (MainCamera->transform->position.z > -40)MainCamera->transform->position.z = -40;

		Timer -= Time.DeltaTime;

		if (Timer < 0) SceneManager::LoadScene(2);
	}
};