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

	float Timer = 360;
	void Update() {
		float xP1;
		float xP2;

		float yP1;
		float yP2;

		xP1 = vPlayers[0]->transform->position.x;
		xP2 = vPlayers[0]->transform->position.x;
		for (int i = 1; i < vPlayers.size(); ++i) {
			if (vPlayers[i]->transform->position.x < xP1) {
				xP1 = vPlayers[i]->transform->position.x;
			}
			else if (vPlayers[i]->transform->position.x > xP2) {
				xP2 = vPlayers[i]->transform->position.x;
			}
		}

		yP1 = vPlayers[0]->transform->position.y;
		yP2 = vPlayers[0]->transform->position.y;
		for (int i = 1; i < vPlayers.size(); ++i) {
			if (vPlayers[i]->transform->position.y < yP1) {
				yP1 = vPlayers[i]->transform->position.y;
			}
			else if (vPlayers[i]->transform->position.y > yP2) {
				yP2 = vPlayers[i]->transform->position.y;
			}
		}

		float dxP1 = xP1 - MainCamera->transform->position.x;
		float dxP2 = xP2 - MainCamera->transform->position.x;

		float Xavg = (xP1 + xP2) / 2;
		float Yavg = (yP1 + yP2) / 2;

		float speed = abs(MainCamera->transform->position.x - Xavg);
		float speedY = abs(MainCamera->transform->position.y - Yavg) / 0.1f;

		if (MainCamera->transform->position.x > Xavg + 0.2f) {
			MainCamera->transform->position.x -= speed * Time.DeltaTime;
		}
		else if (MainCamera->transform->position.x < Xavg - 0.2f) {
			MainCamera->transform->position.x += speed * Time.DeltaTime;
		}

		if (MainCamera->transform->position.y > Yavg) {
			MainCamera->transform->position.y -= speedY * Time.DeltaTime;
		}
		else if (MainCamera->transform->position.y < Yavg) {
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
		//Zoom out
		if (plmax < lmax.x + 3 || pRmax > Rmax.x - 3) {
			MainCamera->transform->position.z *= 1.0009f;
		}
		//Zoom in
		else if (plmax > lmax.x + 15 && pRmax < Rmax.x - 15) {
			MainCamera->transform->position.z *= 0.9990f;
		}

		if (MainCamera->transform->position.z > -40)MainCamera->transform->position.z = -40;

		Timer -= Time.DeltaTime;

		if (Timer < 0) SceneManager::LoadScene(2);
	}
};